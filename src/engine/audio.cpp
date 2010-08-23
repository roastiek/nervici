//#include <vector>
#include <al.h>
#include <alc.h>
#include <iostream>
#include <glibmm/fileutils.h>
#include <glibmm/stringutils.h>

#include "system.h"
#include "settings/pl_info.h"
#include "settings/setting.h"
#include "settings/settings.h"
#include "engine/audio_decoder.h"
#include "engine/mplayer_decoder.h"

#include "engine/audio.h"

using namespace std;
using namespace Glib;

struct MusicFile {
    ustring filename;
    bool played;
};

struct SoundProfile {
    ustring name;
    ustring directory;
};

struct SoundProfileImpl: public SoundProfile {
    vector<ALuint> buffers[ET_Count];
};

struct PlAudio {
    int prof;
    ALuint source;
    int r;
};

static const char* const efect_mask[ET_Count] = {
    "jump",
    "au",
    "self",
    "smileplus",
    "smileminus",
    "wall"};

static const char* const section = "audio";
static const char* const st_sound = "sound";
static const char* const st_music = "music";
static const char* const st_buffer = "buffer";
static const char* const st_bfrCount = "bfrCount";

static ALfloat source_pos[] = {
    0.0,
    0.0,
    0.0};
static ALfloat source_vel[] = {
    0.0,
    0.0,
    0.0};

static ALfloat listener_pos[] = {
    0.0,
    0.0,
    0.0};
static ALfloat listener_vel[] = {
    0.0,
    0.0,
    0.0};
static ALfloat listener_ori[] = {
    0.0,
    0.0,
    -1.0,
    0.0,
    1.0,
    0.0};

Audio::Audio () :
    music_loop (false), music_type (MT_Count) {

}

void Audio::load_setting () {

    Setting& set = settings.app ();

    setting.sound = set.read_int (section, st_sound, 20);
    setting.music = set.read_int (section, st_music, 20);
    setting.buffer = set.read_int (section, st_buffer, 0x10000);
    setting.bfrs_count = set.read_int (section, st_bfrCount, 2);
}

void Audio::save_setting () {

    Setting& set = settings.app ();

    set.write_int (section, st_sound, setting.sound);
    set.write_int (section, st_music, setting.music);
    set.write_int (section, st_buffer, setting.buffer);
    set.write_int (section, st_bfrCount, setting.bfrs_count);
}

int Audio::find_profil (const ustring& name) const {
    for (size_t si = 0; si < sound_profiles.size (); si++) {
        cout << name << " / " << sound_profiles[si]->name << '\n';
        if (name.compare (sound_profiles[si]->name) == 0) {
            return si;
        }
    }
    return -1;
}

bool Audio::music_open (MusicType type) {
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
        f = rand () % music[type].size ();
        if (!music[type][f].played) {
            music[type][f].played = true;
            count++;

            if (music_open_impl (music[type][f].filename)) {
                return true;
            }
        }
    }

    return false;
}

void Audio::initialize () {
    load_setting ();

    initialize_impl ();

    load_sounds ();
    load_music ();
}

void Audio::uninitialize () {
    free_music ();
    free_sounds ();

    uninitialize_impl ();

    save_setting ();
}

void Audio::music_stop () {
    music_stop_impl ();
    music_close_impl ();
    music_loop = false;
}

const ustring& Audio::get_profile (size_t id) const {
    return sound_profiles[id]->name;
}

void Audio::music_play (MusicType type) {
    if (music_is_playing_impl ()) {
        if (music_type == type)
            return;
    }

    music_stop ();

    music_type = type;
    if (!music_open (type))
        return;

    music_play_impl ();

    music_loop = true;
}

void Audio::music_update () {
    if (music_is_playing_impl ()) {
        music_update_impl ();
    } else if (music_loop) {
        music_play (music_type);
    }
}

void Audio::load_music () {
    static const char* const suffixs[MT_Count] = {
        "game/",
        "game/",
        "menu/",
        "stat/"};
    ustring dir_name;
    int mt;

    for (size_t di = 0; di < paths.get_data_dirs_count (); di++) {
        for (mt = 0; mt < MT_Count; mt++) {

            dir_name = paths.get_data_dir (di) + "music/" + suffixs[mt];
            scan_music_dir (dir_name, music[mt], MusicType (mt));
        }
    }
}

void Audio::scan_music_dir (const ustring& path, vector<MusicFile>& files,
        MusicType type) {
    double len;
    ustring file_path;
    MusicFile entry;

    try {

        Dir dir (path);

        for (DirIterator it = dir.begin (); it != dir.end (); it++) {
            if ((*it)[0] == '.')
                continue;

            file_path = path + (*it);

            len = get_sound_length_impl (file_path);

            if (len <= 0)
                continue;
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

void Audio::free_music () {

}

void Audio::scan_sounds_dir (const ustring& path, vector<char>& data) {
    ustring prof_path;
    ustring lower;
    size_t sound_count;

    cout << __func__ << '\n';

    try {
        Dir sound_dir (path);

        for (DirIterator sit = sound_dir.begin (); sit != sound_dir.end (); sit++) {
            if ((*sit)[0] == '.')
                continue;
            SoundProfile* entry;

            prof_path = path + (*sit) + "/";

            entry = create_profile_impl (*sit, prof_path);

            sound_count = 0;

            try {
                Dir prof_dir (prof_path);

                for (DirIterator pit = prof_dir.begin (); pit
                        != prof_dir.end (); pit++) {
                    if ((*pit)[0] == '.')
                        continue;
                    lower = ustring (*pit).lowercase ();

                    for (int eti = 0; eti < ET_Count; eti++) {
                        if (str_has_prefix (lower, efect_mask[eti])) {
                            sound_count += load_sound_impl (entry, EffectType (
                                    eti), prof_path + (*pit), data);
                        }
                    }

                }

            } catch (FileError) {
            }

            if (sound_count > 0) {
                sound_profiles.push_back (entry);
            } else {
                delete entry;
            }
        }

    } catch (FileError) {
    }
}

void Audio::load_sounds () {
    vector<char> data;
    data.resize (0xffff);

    for (size_t di = 0; di < paths.get_data_dirs_count (); di++) {
        ustring dir = paths.get_data_dir (di) + "sounds/";
        scan_sounds_dir (dir, data);
    }
}

void Audio::free_sounds () {
}

SoundProfile& Audio::get_sprofile (size_t index) {
    return *sound_profiles[index];
}

const SoundProfile& Audio::get_sprofile (size_t index) const {
    return *sound_profiles[index];
}

class AudioOpenAL: public Audio {
private:
    ALCdevice* device;

    ALCcontext* context;

    ALuint* music_buffers;

    ALuint music_source;

    char* music_data;

    AudioDecoder* music_decoder;

    vector<PlAudio> sounds;

    void init_music ();

    void uninit_music ();

    void init_sounds ();

    void uninit_sounds ();

    int music_stream (ALuint buffer);

protected:
    void initialize_impl ();

    void uninitialize_impl ();

    void music_play_impl ();

    void music_stop_impl ();

    bool music_open_impl (const ustring& filename);

    void music_close_impl ();

    bool music_is_playing_impl ();

    double get_sound_length_impl (const ustring& filename) const;

    SoundProfileImpl* create_profile_impl (const ustring& name,
            const ustring& directory) const;

    SoundProfileImpl& get_sprofile (size_t index);

    const SoundProfileImpl& get_sprofile (size_t index) const;

    bool load_sound_impl (SoundProfile* prof, EffectType effect,
            const Glib::ustring& filename, vector<char>& data) const;

    void music_update_impl ();

public:
    AudioOpenAL ();

    void music_update ();

    void music_set_rate (float rate);

    void load_players (const std::vector<const PlInfo*>& infos);

    void free_players ();

    void play_effect (plid_tu plid, EffectType effect);
};

void AudioOpenAL::init_music () {
    music_data = new char[setting.buffer];
    music_buffers = new ALuint[setting.bfrs_count];

    alGenBuffers (setting.bfrs_count, music_buffers);
    alGenSources (1, &music_source);

    alSourcefv (music_source, AL_POSITION, source_pos);
    alSourcefv (music_source, AL_VELOCITY, source_vel);
    alSourcef (music_source, AL_GAIN, setting.music / 20.0);
}

void AudioOpenAL::uninit_music () {

    alDeleteBuffers (setting.bfrs_count, music_buffers);
    alDeleteSources (1, &music_source);

    delete[] music_data;
    delete[] music_buffers;
}

void AudioOpenAL::init_sounds () {

}

void AudioOpenAL::uninit_sounds () {
    for (size_t pi = 0; pi < sound_profiles.size (); pi++) {
        SoundProfileImpl& spi = get_sprofile (pi);
        for (int eti = 0; eti < ET_Count; eti++) {
            if (spi.buffers[eti].size () > 0) {
                alDeleteBuffers (spi.buffers[eti].size (),
                        spi.buffers[eti].data ());
                spi.buffers[eti].clear ();
            }
        }
    }
}

int AudioOpenAL::music_stream (ALuint buffer) {
    size_t size = music_decoder->read (music_data, setting.buffer);
    if (size > 0) {

        alBufferData (buffer, music_decoder->get_format (), music_data, size,
                music_decoder->get_frequency ());
    }
    return size;
}

void AudioOpenAL::initialize_impl () {
    device = alcOpenDevice (NULL);
    context = alcCreateContext (device, NULL);
    alcMakeContextCurrent (context);

    alListenerfv (AL_POSITION, listener_pos);
    alListenerfv (AL_VELOCITY, listener_vel);
    alListenerfv (AL_ORIENTATION, listener_ori);

    init_sounds ();
    init_music ();
}

void AudioOpenAL::uninitialize_impl () {
    uninit_music ();
    uninit_sounds ();

    alcMakeContextCurrent (NULL);
    alcDestroyContext (context);
    alcCloseDevice (device);
}

void AudioOpenAL::music_play_impl () {
    for (int bi = 0; bi < setting.bfrs_count; bi++) {
        if (music_stream (music_buffers[bi]) <= 0) {

            break;
        }
        alSourceQueueBuffers (music_source, 1, &music_buffers[bi]);
    }

    alSourcePlay (music_source);
}

void AudioOpenAL::music_stop_impl () {
    int queued;
    ALuint buffer;

    alSourceStop (music_source);
    alGetSourcei (music_source, AL_BUFFERS_QUEUED, &queued);

    for (; queued > 0; queued--) {
        alSourceUnqueueBuffers (music_source, 1, &buffer);
    }
}

bool AudioOpenAL::music_open_impl (const ustring& filename) {
    music_decoder = new MplayerDecoder ();
    if (!music_decoder->open (filename)) {
        delete music_decoder;
        music_decoder = NULL;
        return false;
    }
    return true;
}

void AudioOpenAL::music_close_impl () {
    if (music_decoder != NULL) {
        delete music_decoder;
        music_decoder = NULL;
    }
}

bool AudioOpenAL::music_is_playing_impl () {
    ALenum state;
    alGetSourcei (music_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

double AudioOpenAL::get_sound_length_impl (const ustring& filename) const {
    MplayerDecoder dec;
    return dec.get_length (filename);
}

SoundProfileImpl* AudioOpenAL::create_profile_impl (const ustring& name,
        const ustring& directory) const {
    SoundProfileImpl* result = new SoundProfileImpl ();
    result->name = name;
    result->directory = directory;
    return result;
}

SoundProfileImpl& AudioOpenAL::get_sprofile (size_t index) {
    return *static_cast<SoundProfileImpl*> (sound_profiles[index]);
}

const SoundProfileImpl& AudioOpenAL::get_sprofile (size_t index) const {
    return *static_cast<SoundProfileImpl*> (sound_profiles[index]);
}

bool AudioOpenAL::load_sound_impl (SoundProfile* prof, EffectType effect,
        const Glib::ustring& filename, vector<char>& data) const {
    MplayerDecoder dec;
    SoundProfileImpl* entry = static_cast<SoundProfileImpl*> (prof);

    if (dec.open (filename)) {
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

        ALuint sound_buffer;
        alGenBuffers (1, &sound_buffer);
        alBufferData (sound_buffer, dec.get_format (), data.data (), whole,
                dec.get_frequency ());
        entry->buffers[effect].push_back (sound_buffer);
        return true;
    }
    return false;
}

AudioOpenAL::AudioOpenAL () :
    Audio (), music_data (NULL), music_decoder (NULL) {

}

void AudioOpenAL::music_update_impl () {
    int processed;
    ALuint buffer;

    alGetSourcei (music_source, AL_BUFFERS_PROCESSED, &processed);

    for (; processed > 0; processed--) {
        alSourceUnqueueBuffers (music_source, 1, &buffer);
        if (music_stream (buffer) > 0)
            alSourceQueueBuffers (music_source, 1, &buffer);
    }
}

void AudioOpenAL::music_set_rate (float rate) {
    alSourcef (music_source, AL_PITCH, rate);
}

void AudioOpenAL::load_players (const std::vector<const PlInfo*>& infos) {
    sounds.clear ();

    for (size_t si = 0; si < infos.size (); si++) {
        const PlInfo& info = *infos[si];
        PlAudio entry;
        alGenSources (1, &entry.source);
        alSourcef (entry.source, AL_PITCH, (info.pitch + 5.0) / 10.0);
        alSourcef (entry.source, AL_GAIN, setting.sound / 20.0);
        alSourcefv (entry.source, AL_VELOCITY, source_vel);
        alSourcefv (entry.source, AL_POSITION, source_pos);
        cout << info.profil << '\n';
        entry.prof = find_profil (info.profil);
        sounds.push_back (entry);
    }
}

void AudioOpenAL::free_players () {
    for (size_t si = 0; si < sounds.size (); si++) {

        alSourceStop (sounds[si].source);
        alDeleteSources (1, &sounds[si].source);
    }
}

void AudioOpenAL::play_effect (plid_tu plid, EffectType effect) {
    ALint play;
    ALuint source;

    if (sounds[plid].prof >= 0) {
        alGetSourcei (sounds[plid].source, AL_SOURCE_STATE, &play);

        SoundProfileImpl& spi = get_sprofile (sounds[plid].prof);
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

static AudioOpenAL instance;

Audio& audio = instance;
