#include <al.h>
#include <alc.h>
#include <vector>
#include <iostream>
#include <glibmm/fileutils.h>
#include <glibmm/stringutils.h>

#include "system.h"
#include "settings/pl_infos.h"
#include "settings/setting.h"
#include "game/game.h"
#include "engine/audio_decoder.h"
#include "engine/mplayer_decoder.h"

#include "engine/audio.h"

using namespace std;
using namespace Glib;

namespace Audio {

struct AudioSetting {
    int sound;
    int music;
    int buffer;
    int bfrs_count;
};

struct MusicFile {
    ustring filename;
    bool played;
};

struct SoundProfile {
    ustring name;
    ustring directory;
    vector<ALuint> buffers[ET_Count];
};

struct PlAudio {
    int prof;
    ALuint source;
    int r;
};

#define WAVS_COUNT 8


static const char* const efect_mask[ET_Count] = {
    "jump", "au", "self", "smileplus", "smileminus", "wall"
};

static const char* const section = "audio";
static const char* const st_sound = "sound";
static const char* const st_music = "music";
static const char* const st_buffer = "buffer";
static const char* const st_bfrCount = "bfrCount";

static ALCdevice* device;
static ALCcontext* context;
static AudioSetting setting;

static vector<SoundProfile> sound_profiles;
static vector<PlAudio> sounds;

static bool music_loop = false;
static ALuint* music_buffers;
static ALuint music_source;
static char* music_data = NULL;
static MusicType music_type = MT_Count;
static AudioDecoder* music_decoder = NULL;
static vector<MusicFile> music[MT_Count];


static ALfloat source_pos[] = {0.0, 0.0, 0.0};
static ALfloat source_vel[] = {0.0, 0.0, 0.0};

static ALfloat listener_pos[] = {0.0, 0.0, 0.0};
static ALfloat listener_vel[] = {0.0, 0.0, 0.0};
static ALfloat listener_ori[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

static void scan_sounds_dir (const ustring& path, vector<SoundProfile>& profiles,
        vector<char>& data) {
    ustring prof_path;
    ustring lower;
    size_t sound_count;
    ALuint sound_buffer;

    cout << __func__ << '\n';

    try {
        Dir sound_dir (path);

        for (DirIterator sit = sound_dir.begin (); sit != sound_dir.end (); sit++) {
            if ((*sit)[0] == '.') continue;
            SoundProfile entry;

            prof_path = path + (*sit) + "/";

            entry.directory = prof_path;
            entry.name = (*sit);

            sound_count = 0;

            try {
                Dir prof_dir (prof_path);

                for (DirIterator pit = prof_dir.begin (); pit != prof_dir.end (); pit++) {
                    if ((*pit)[0] == '.') continue;
                    lower = ustring (*pit).lowercase ();

                    for (int eti = 0; eti < ET_Count; eti++) {
                        if (str_has_prefix (lower, efect_mask[eti])) {
                            MplayerDecoder dec;
                            if (dec.open (prof_path + (*pit))) {
                                size_t pos = 0;
                                size_t avai = data.size ();
                                size_t read = dec.read (&data.data ()[pos], avai);
                                size_t whole = 0;
                                while (read == avai) {
                                    whole += read;
                                    size_t new_size = data.size () * 2 + 1;
                                    data.resize (new_size);
                                    pos += read;
                                    avai = data.size () - avai;
                                    read = dec.read (&data.data ()[pos], avai);
                                }
                                whole += read;

                                alGenBuffers (1, &sound_buffer);
                                alBufferData (sound_buffer, dec.get_format (),
                                        data.data (), whole, dec.get_frequency ());
                                entry.buffers[eti].push_back (sound_buffer);
                                sound_count++;
                            }
                        }
                    }

                }

            } catch (FileError) {
            }

            if (sound_count > 0) {
                profiles.push_back (entry);
            }
        }

    } catch (FileError) {
    }
}

static void scan_music_dir (const ustring& path, vector<MusicFile>& files, MusicType type) {
    double len;
    ustring file_path;
    MusicFile entry;

    try {

        Dir dir (path);

        for (DirIterator it = dir.begin (); it != dir.end (); it++) {
            if ((*it)[0] == '.') continue;

            file_path = path + (*it);

            MplayerDecoder dec;

            len = dec.get_length (file_path);

            if (len <= 0) continue;
            if (type == MT_GameShort && len > 150) {
                continue;
            }
            if (type == MT_GameLong && len <= 150) {
                continue;
            }

            entry.filename = file_path;
            entry.played = false;
            files.push_back (entry);
        }

    } catch (FileError) {
    }
}

static void init_wavs () {
    cout << __func__ << '\n';

    vector<char> data;
    data.resize (0xffff);

    for (size_t di = 0; di < System::get_data_dirs_count (); di++) {
        ustring dir = System::get_data_dir (di) + "sounds/";
        scan_sounds_dir (dir, sound_profiles, data);
    }
}

static void free_wavs () {
    for (size_t pi = 0; pi < sound_profiles.size (); pi++) {
        SoundProfile& spi = sound_profiles[pi];
        for (int eti = 0; eti < ET_Count; eti++) {
            if (spi.buffers[eti].size () > 0)
                alDeleteBuffers (spi.buffers[eti].size (), spi.buffers[eti].data ());
        }
    }
}

static void init_music () {
    static const char* const suffixs[MT_Count] = {"game/", "game/", "menu/", "stat/"};
    ustring dir_name;
    int mt;

    cout << __func__ << '\n';

    for (size_t di = 0; di < System::get_data_dirs_count (); di++) {
        for (mt = 0; mt < MT_Count; mt++) {

            dir_name = System::get_data_dir (di) + "music/" + suffixs[mt];
            scan_music_dir (dir_name, music[mt], MusicType (mt));
        }
    }

    music_data = new char[setting.buffer];
    music_buffers = new ALuint[setting.bfrs_count];

    alGenBuffers (setting.bfrs_count, music_buffers);
    alGenSources (1, &music_source);

    alSourcefv (music_source, AL_POSITION, source_pos);
    alSourcefv (music_source, AL_VELOCITY, source_vel);
    alSourcef (music_source, AL_GAIN, setting.music / 20.0);

}

static void free_music () {

    alDeleteBuffers (setting.bfrs_count, music_buffers);
    alDeleteSources (1, &music_source);

    delete [] music_data;
    delete [] music_buffers;
}

static void load_setting () {

    Setting& set = Settings::get_app_setting ();

    setting.sound = set.read_int (section, st_sound, 20);
    setting.music = set.read_int (section, st_music, 20);
    setting.buffer = set.read_int (section, st_buffer, 0x10000);
    setting.bfrs_count = set.read_int (section, st_bfrCount, 2);
}

static void save_setting () {

    Setting& set = Settings::get_app_setting ();

    set.write_int (section, st_sound, setting.sound);
    set.write_int (section, st_music, setting.music);
    set.write_int (section, st_buffer, setting.buffer);
    set.write_int (section, st_bfrCount, setting.bfrs_count);
}

static int find_profil (const ustring& name) {
    for (size_t si = 0; si < sound_profiles.size (); si++) {
        if (name.compare (sound_profiles[si].name) == 0) {

            return si;
        }
    }
    return -1;
}

static bool music_open (MusicType type) {
    int f;
    size_t count;

    count = 0;
    for (size_t mi = 0; mi < music[type].size (); mi++) {
        count += music[type][mi].played;
    }

    if (count == music[type].size ()) {
        for (size_t mi = 0; mi < music[type].size (); mi++) {
            music[type][mi].played = false;
        }
        count = 0;
    }

    while (count < music[type].size ()) {
        cout << "music open " << count << '/' << music[type].size () << '\n';
        f = rand () % music[type].size ();
        if (!music[type][f].played) {
            music[type][f].played = true;
            count++;

            music_decoder = new MplayerDecoder ();
            if (!music_decoder->open (music[type][f].filename)) {
                delete music_decoder;
                music_decoder = NULL;

                continue;
            }

            return true;
        }
    }

    return false;
}

static void music_close () {
    if (music_decoder != NULL) {

        delete music_decoder;
        music_decoder = NULL;
    }
}

static int music_stream (ALuint buffer) {
    size_t size = music_decoder->read (music_data, setting.buffer);
    if (size > 0) {

        alBufferData (buffer, music_decoder->get_format (), music_data, size, music_decoder->get_frequency ());
    }
    return size;
}

void initialize () {

    cout << __func__ << '\n';
    device = alcOpenDevice (NULL);
    context = alcCreateContext (device, NULL);
    alcMakeContextCurrent (context);

    alListenerfv (AL_POSITION, listener_pos);
    alListenerfv (AL_VELOCITY, listener_vel);
    alListenerfv (AL_ORIENTATION, listener_ori);

    load_setting ();

    init_wavs ();
    init_music ();
}

void uninitialize () {

    save_setting ();

    free_music ();
    free_wavs ();

    alcMakeContextCurrent (NULL);
    alcDestroyContext (context);
    alcCloseDevice (device);
}

void load_players (const GameInfo& info) {
    sounds.clear ();

    for (size_t si = 0; si < 16; si++) {
        if (info.pl_ids[si] >= 0) {

            PlAudio entry;
            alGenSources (1, &entry.source);
            alSourcef (entry.source, AL_PITCH, (PlInfos::get (info.pl_ids[si]).pitch + 5.0) / 10.0);
            alSourcef (entry.source, AL_GAIN, setting.sound / 20.0);
            alSourcefv (entry.source, AL_VELOCITY, source_vel);
            alSourcefv (entry.source, AL_POSITION, source_pos);
            entry.prof = find_profil (PlInfos::get (info.pl_ids[si]).profil);
            sounds.push_back (entry);
        }
    }
}

void free_players () {
    for (size_t si = 0; si < sounds.size (); si++) {

        alSourceStop (sounds[si].source);
        alDeleteSources (1, &sounds[si].source);
    }
}

void play_effect (plid_tu plid, EffectType effect) {
    ALint play;
    //SoundProfile *spi;
    ALuint source;

    if (sounds[plid].prof >= 0) {
        alGetSourcei (sounds[plid].source, AL_SOURCE_STATE, &play);

        SoundProfile& spi = sound_profiles[sounds[plid].prof];
        source = sounds[plid].source;

        if (play != AL_PLAYING) {
            if (spi.buffers[effect].size () > 0) {

                int index = sounds[plid].r % spi.buffers[effect].size ();
                alSourcei (source, AL_BUFFER, spi.buffers[effect][index]);

            }
            sounds[plid].r++;

            alSourcePlay (source);
        }
    }
}

void music_play (MusicType type) {
    ALenum state;
    int b;

    alGetSourcei (music_source, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        if (music_type == type) return;
    }

    music_stop ();

    music_type = type;
    if (!music_open (type)) return;

    for (b = 0; b < setting.bfrs_count; b++) {
        if (music_stream (music_buffers[b]) <= 0) {

            break;
        }
        alSourceQueueBuffers (music_source, 1, &music_buffers[b]);
    }

    alSourcePlay (music_source);
    music_loop = true;
}

void music_stop () {
    int queued;
    ALuint buffer;

    alSourceStop (music_source);
    alGetSourcei (music_source, AL_BUFFERS_QUEUED, &queued);

    for (; queued > 0; queued--) {

        alSourceUnqueueBuffers (music_source, 1, &buffer);
    }

    music_close ();
    music_loop = false;
}

void music_update () {
    int processed;
    ALenum state;
    ALuint buffer;

    alGetSourcei (music_source, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {

        alGetSourcei (music_source, AL_BUFFERS_PROCESSED, &processed);

        for (; processed > 0; processed--) {
            alSourceUnqueueBuffers (music_source, 1, &buffer);
            if (music_stream (buffer) > 0)
                alSourceQueueBuffers (music_source, 1, &buffer);
        }
    } else if (music_loop) {

        music_play (music_type);
    }

}

void music_set_rate (float rate) {

    alSourcef (music_source, AL_PITCH, rate);
}

size_t get_profiles_count () {

    return sound_profiles.size ();
}

const ustring& get_profile (size_t id) {
    return sound_profiles[id].name;
}

}
