#include <al.h>
#include <alc.h>
#include <SDL.h>
#include <dirent.h>
#include <vorbis/vorbisfile.h>
#include <AL/al.h>
#include <vector>
#include <iostream>
using namespace std;

#include "system.h"
#include "settings/pl_infos.h"
#include "settings/setting.h"
#include "game/game.h"

#include "engine/audio.h"

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

struct PlAudio {
    int prof;
    ALuint source;
    int r;
};

#define WAVS_COUNT 8

struct SoundProfile {
    ustring name;
    ustring directory;
    ALuint buffers[WAVS_COUNT];
};

static const char* const wavs[] = {
    "/hop.wav", "/jau1.wav", "/jau2.wav", "/self.wav", "/smileplus.wav",
    "/smileminus.wav", "/wall1.wav", "/wall2.wav"
};

static const char* const section = "audio";
static const char* const st_sound = "sound";
static const char* const st_music = "music";
static const char* const st_buffer = "buffer";
static const char* const st_bfrCount = "bfrCount";

static float music_rate = 1.0;
static bool music_loop = false;
static bool music_opened = false;
static OggVorbis_File music_file;
static vorbis_info* music_info;
static ALuint* music_buffers;
static ALuint music_source;
static ALenum music_format;
static MusicType music_type = MT_Count;
static char* music_loader = NULL;
static ALCdevice* device;
static ALCcontext* context;
static AudioSetting setting;
static vector<SoundProfile> sound_profiles;
static vector<PlAudio> sounds;
static vector<MusicFile> music[MT_Count];

static ALfloat source_pos[] = {0.0, 0.0, 0.0};
static ALfloat source_vel[] = {0.0, 0.0, 0.0};

static ALfloat listener_pos[] = {0.0, 0.0, 0.0};
static ALfloat listener_vel[] = {0.0, 0.0, 0.0};
static ALfloat listener_ori[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

static void scan_sounds_dir (const ustring& path, vector<SoundProfile>& profiles) {
    int w;
    int valid;
    DIR *dir;
    struct dirent *ent;
    FILE *file;
    ustring prof_path;
    ustring wav_path;
    SoundProfile entry;

    dir = opendir (path.c_str ());
    if (dir == NULL) return;

    for (ent = readdir (dir); ent != NULL; ent = readdir (dir)) {
        if (ent->d_name[0] == '.') continue;

        prof_path = path + '/' + ent->d_name;

        valid = 1;
        for (w = 0; w < WAVS_COUNT; w++) {
            wav_path = prof_path + wavs[w];

            file = fopen (wav_path.c_str (), "rb");
            if (file == NULL) {
                valid = 0;
                break;
            }
            fclose (file);
        }

        if (valid) {
            entry.directory = prof_path;
            entry.name = ent->d_name;
            profiles.push_back (entry);
        }
    }

    closedir (dir);
}

static ALuint load_wav (const ustring& filename) {
    SDL_AudioSpec spec;
    Uint8 *data;
    Uint32 len;
    ALuint result;

    alGenBuffers (1, &result);

    if (SDL_LoadWAV (filename.c_str (), &spec, &data, &len) == NULL) return result;
    switch (spec.channels) {
    case 1:
        switch (spec.format) {
        case AUDIO_S8:
            alBufferData (result, AL_FORMAT_MONO8, data, len, spec.freq);
            break;
        case AUDIO_S16LSB:
        case AUDIO_S16MSB:
            alBufferData (result, AL_FORMAT_MONO16, data, len, spec.freq);
            break;
        }
        break;
    case 2:
        switch (spec.format) {
        case AUDIO_S8:
            alBufferData (result, AL_FORMAT_STEREO8, data, len, spec.freq);
            break;
        case AUDIO_S16LSB:
        case AUDIO_S16MSB:
            alBufferData (result, AL_FORMAT_STEREO16, data, len, spec.freq);
            break;
        }
        break;
    default:
        break;
    }

    SDL_FreeWAV (data);

    return result;
}

static void load_buffers (vector<SoundProfile>& profiles) {
    int w;
    ustring filename;

    for (size_t pi = 0; pi < profiles.size (); pi++) {
        SoundProfile& spi = profiles[pi];

        for (w = 0; w < WAVS_COUNT; w++) {
            filename = spi.directory + wavs[w];
            spi.buffers[w] = load_wav (filename);
        }
    }
}

static double ogg_get_length (const ustring& filename) {
    OggVorbis_File file;
    double result;
    size_t len;
    char *fn;

    len = filename.length ();
    fn = (char *) alloca (len + 1);
    memcpy (fn, filename.c_str (), len + 1);

    if (ov_fopen (fn, &file)) return -1;

    result = ov_time_total (&file, -1);
    ov_clear (&file);

    return result;
}

static void scan_music_dir (const ustring& path, vector<MusicFile>& files, MusicType type) {
    DIR *dir;
    struct dirent *ent;
    double len;
    ustring file_path;
    MusicFile entry;

    dir = opendir (path.c_str ());
    if (dir == NULL) return;

    for (ent = readdir (dir); ent != NULL; ent = readdir (dir)) {
        if (ent->d_name[0] == '.') continue;

        file_path = path + '/' + ent->d_name;

        len = ogg_get_length (file_path);

        if (len <= 0) continue;
        if (type == MT_Short && len > 150) {
            continue;
        }
        if (type == MT_Long && len <= 150) {
            continue;
        }

        entry.filename = file_path;
        entry.played = false;
        files.push_back (entry);
    }

    closedir (dir);
}

static void init_wavs () {
    cout << __func__ << '\n';

    for (size_t di = 0; di < System::get_data_dirs_count(); di++) {
        ustring dir = System::get_data_dir(di)+ "sounds/";
        scan_sounds_dir (dir, sound_profiles);
    }

    load_buffers (sound_profiles);
}

static void free_wavs () {
    for (size_t pi = 0; pi < sound_profiles.size (); pi++) {
        SoundProfile& spi = sound_profiles[pi];
        alDeleteBuffers (WAVS_COUNT, spi.buffers);
    }
}

static void init_music () {
    static const char* const suffixs[MT_Count] = {"game/", "game/", "menu/", "stat/"};
    ustring dir_name;
    MusicType mt;

    cout << __func__ << '\n';

    for (size_t di = 0; di < System::get_data_dirs_count(); di++) {
        for (mt = MT_Short; mt < MT_Count; mt++) {
            dir_name = System::get_data_dir(di) + "music/" + suffixs[mt];
            scan_music_dir (dir_name, music[mt], mt);
        }
    }

    music_loader = new char[setting.buffer];
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

    delete [] music_loader;
    delete [] music_buffers;
}

static void load_setting () {
    cout << __func__ << '\n';

    setting.sound = Setting::read_int (section, st_sound, 20);
    setting.music = Setting::read_int (section, st_music, 20);
    setting.buffer = Setting::read_int (section, st_buffer, 0x10000);
    setting.bfrs_count = Setting::read_int (section, st_bfrCount, 2);
}

static void save_setting () {
    Setting::write_int (section, st_sound, setting.sound);
    Setting::write_int (section, st_music, setting.music);
    Setting::write_int (section, st_buffer, setting.buffer);
    Setting::write_int (section, st_bfrCount, setting.bfrs_count);
}

static int find_profil (const ustring& name) {
    for (size_t si = 0; si < sound_profiles.size (); si++) {
        if (strcasecmp (name.c_str (), sound_profiles[si].name.c_str ()) == 0) {
            return si;
        }
    }
    return -1;
}

static bool music_open (MusicType type) {
    int f;
    int op;
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

            op = ov_fopen ((char *) music[type][f].filename.c_str (), &music_file);
            if (op) continue;

            music_info = ov_info (&music_file, -1);

            if (music_info->channels == 1)
                music_format = AL_FORMAT_MONO16;
            else if (music_info->channels == 2)
                music_format = AL_FORMAT_STEREO16;
            else {
                ov_clear (&music_file);
                continue;
            }

            music_opened = true;
            return true;
        }
    }

    return false;
}

static void music_close () {
    if (music_opened) {
        ov_clear (&music_file);
        music_opened = false;
    }
}

static int music_stream (ALuint buffer) {
    int size = 0;
    int section;
    int result;
    int bs;

    bs = setting.buffer / 2;
    while (bs - 1024 >= (setting.buffer / 2) * music_rate) bs -= 1024;
    while (bs < (setting.buffer / 2) * music_rate) bs += 1024;

    while (size < bs) {
        result = ov_read (&music_file, music_loader + size, bs - size, 0, 2, 1, &section);

        if (result > 0) size += result;
        else return result;
    }

    alBufferData (buffer, music_format, music_loader, size, music_info->rate * music_rate);
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
            switch (effect) {
            case ET_Hop:
                alSourcei (source, AL_BUFFER, spi.buffers[0]);
                break;
            case ET_Jau:
                alSourcei (source, AL_BUFFER, spi.buffers[1 + (sounds[plid].r % 2)]);
                sounds[plid].r++;
                break;
            case ET_Self:
                alSourcei (source, AL_BUFFER, spi.buffers[3]);
                break;
            case ET_SmilePlus:
                alSourcei (source, AL_BUFFER, spi.buffers[4]);
                break;
            case ET_SmileMinus:
                alSourcei (source, AL_BUFFER, spi.buffers[5]);
                break;
            case ET_Wall:
                alSourcei (source, AL_BUFFER, spi.buffers[6 + (sounds[plid].r % 2)]);
                sounds[plid].r++;
                break;
            }

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
    music_rate = rate;
}

size_t get_profiles_count () {
    return sound_profiles.size ();
}

const ustring& get_profile (size_t id) {
    return sound_profiles[id].name;
}

}
