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
#include "engine/sound_profile.h"

#include "engine/audio.h"

using namespace std;
using namespace Glib;

class AudioOpenAL: public Audio {
private:
    struct PlAudio {
        int prof;
        ALuint source;
        int r;
    };

    struct SoundProfileImpl: public SoundProfile {
        vector<ALuint> buffers[ET_Count];
    };

    struct MusicFile {
        ustring filename;
        bool played;
    };

    bool music_loop;

    MusicType music_type;

    ALCdevice* device;

    ALCcontext* context;

    ALuint* music_buffers;

    ALuint music_source;

    char* music_data;

    AudioDecoder* music_decoder;

    vector<PlAudio> sounds;

    std::vector<MusicFile> music[MT_Count];

    struct {
        int sound;
        int music;
        int buffer;
        int bfrs_count;
    } setting;

    static ALfloat source_pos[];

    static ALfloat source_vel[];

    static ALfloat listener_pos[];

    static ALfloat listener_vel[];

    static ALfloat listener_ori[];

    static AudioOpenAL instance;

    static const ustring efect_mask[ET_Count];

    static const ustring section;
    
    static const ustring st_sound;
    
    static const ustring st_music;
    
    static const ustring st_buffer;
    
    static const ustring st_bfrCount;

    AudioOpenAL ();

    void load_setting ();

    void load_sounds ();

    void load_music ();

    void save_setting ();

    void free_music ();

    void free_sounds ();

    void scan_music_dir (const Glib::ustring& path,
            std::vector<MusicFile>& files, MusicType type);

    bool music_open (MusicType type);

    int music_stream (ALuint buffer);

    void scan_sounds_dir (const Glib::ustring& path, std::vector<char>& data);

    int find_profil (const Glib::ustring& name) const;

    bool music_is_playing ();

    SoundProfileImpl& get_sprofile (size_t index);

    const SoundProfileImpl& get_sprofile (size_t index) const;

    bool load_sound (SoundProfileImpl* prof, EffectType effect,
            const Glib::ustring& filename, vector<char>& data) const;

public:
    void initialize ();

    void uninitialize ();

    void music_play (MusicType type);

    void music_stop ();

    void music_update ();

    void music_set_rate (float rate);

    void load_players (const std::vector<const PlInfo*>& infos);

    void free_players ();

    void play_effect (plid_tu plid, EffectType effect);

    static AudioOpenAL& get_instance ();
};

ALfloat AudioOpenAL::source_pos[] = {
    0.0,
    0.0,
    0.0};

ALfloat AudioOpenAL::source_vel[] = {
    0.0,
    0.0,
    0.0};

ALfloat AudioOpenAL::listener_pos[] = {
    0.0,
    0.0,
    0.0};

ALfloat AudioOpenAL::listener_vel[] = {
    0.0,
    0.0,
    0.0};

ALfloat AudioOpenAL::listener_ori[] = {
    0.0,
    0.0,
    -1.0,
    0.0,
    1.0,
    0.0};

const ustring AudioOpenAL::efect_mask[ET_Count] = {
    "jump",
    "au",
    "self",
    "smileplus",
    "smileminus",
    "wall"};

const ustring AudioOpenAL::section = "audio";

const ustring AudioOpenAL::st_sound = "sound";

const ustring AudioOpenAL::st_music = "music";

const ustring AudioOpenAL::st_buffer = "buffer";

const ustring AudioOpenAL::st_bfrCount = "bfrCount";

AudioOpenAL AudioOpenAL::instance;

AudioOpenAL::AudioOpenAL () :
    music_loop (false), music_type (MT_Count), music_data (NULL),
            music_decoder (NULL) {

}

void AudioOpenAL::load_setting () {

    Setting& set = settings.app ();

    setting.sound = set.read_int (section, st_sound, 20);
    setting.music = set.read_int (section, st_music, 20);
    setting.buffer = set.read_int (section, st_buffer, 0x10000);
    setting.bfrs_count = set.read_int (section, st_bfrCount, 2);
}

void AudioOpenAL::save_setting () {

    Setting& set = settings.app ();

    set.write_int (section, st_sound, setting.sound);
    set.write_int (section, st_music, setting.music);
    set.write_int (section, st_buffer, setting.buffer);
    set.write_int (section, st_bfrCount, setting.bfrs_count);
}

int AudioOpenAL::find_profil (const ustring& name) const {
    for (size_t si = 0; si < sound_profiles.size (); si++) {
        cout << name << " / " << sound_profiles[si]->name << '\n';
        if (name.compare (sound_profiles[si]->name) == 0) {
            return si;
        }
    }
    return -1;
}

bool AudioOpenAL::music_open (MusicType type) {
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

            music_decoder = new MplayerDecoder ();
            if (music_decoder->open (music[type][f].filename)) {
                return true;
            }
            delete music_decoder;
            music_decoder = NULL;
        }
    }

    return false;
}

void AudioOpenAL::initialize () {
    load_setting ();

    device = alcOpenDevice (NULL);
    context = alcCreateContext (device, NULL);
    alcMakeContextCurrent (context);

    alListenerfv (AL_POSITION, listener_pos);
    alListenerfv (AL_VELOCITY, listener_vel);
    alListenerfv (AL_ORIENTATION, listener_ori);

    load_sounds ();
    load_music ();
}

void AudioOpenAL::uninitialize () {
    free_music ();
    free_sounds ();

    alcMakeContextCurrent (NULL);
    alcDestroyContext (context);
    alcCloseDevice (device);

    save_setting ();
}

void AudioOpenAL::music_stop () {
    int queued;
    ALuint buffer;

    alSourceStop (music_source);
    alGetSourcei (music_source, AL_BUFFERS_QUEUED, &queued);

    for (; queued > 0; queued--) {
        alSourceUnqueueBuffers (music_source, 1, &buffer);
    }

    if (music_decoder != NULL) {
        delete music_decoder;
        music_decoder = NULL;
    }
    music_loop = false;
}

void AudioOpenAL::music_play (MusicType type) {
    if (music_is_playing ()) {
        if (music_type == type)
            return;
    }

    music_stop ();

    music_type = type;
    if (!music_open (type))
        return;

    for (int bi = 0; bi < setting.bfrs_count; bi++) {
        if (music_stream (music_buffers[bi]) <= 0) {

            break;
        }
        alSourceQueueBuffers (music_source, 1, &music_buffers[bi]);
    }

    alSourcePlay (music_source);

    music_loop = true;
}

void AudioOpenAL::music_update () {
    if (music_is_playing ()) {
        int processed;
        ALuint buffer;

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

void AudioOpenAL::load_music () {
    static const char* const suffixs[MT_Count] = {
        "game/",
        "game/",
        "menu/",
        "stat/"};
    ustring dir_name;
    int mt;

    music_data = new char[setting.buffer];
    music_buffers = new ALuint[setting.bfrs_count];

    alGenBuffers (setting.bfrs_count, music_buffers);
    alGenSources (1, &music_source);

    alSourcefv (music_source, AL_POSITION, source_pos);
    alSourcefv (music_source, AL_VELOCITY, source_vel);
    alSourcef (music_source, AL_GAIN, setting.music / 20.0);

    for (size_t di = 0; di < paths.get_data_dirs_count (); di++) {
        for (mt = 0; mt < MT_Count; mt++) {

            dir_name = paths.get_data_dir (di) + "music/" + suffixs[mt];
            scan_music_dir (dir_name, music[mt], MusicType (mt));
        }
    }
}

void AudioOpenAL::scan_music_dir (const ustring& path,
        vector<MusicFile>& files, MusicType type) {
    double len;
    ustring file_path;
    MusicFile entry;

    try {

        Dir dir (path);

        for (DirIterator it = dir.begin (); it != dir.end (); it++) {
            if ((*it)[0] == '.')
                continue;

            file_path = path + (*it);

            MplayerDecoder dec;
            len = dec.get_length (file_path);

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

void AudioOpenAL::free_music () {
    alDeleteBuffers (setting.bfrs_count, music_buffers);
    alDeleteSources (1, &music_source);

    delete[] music_data;
    delete[] music_buffers;
}

void AudioOpenAL::scan_sounds_dir (const ustring& path, vector<char>& data) {
    ustring prof_path;
    ustring lower;
    size_t sound_count;

    cout << __func__ << '\n';

    try {
        Dir sound_dir (path);

        for (DirIterator sit = sound_dir.begin (); sit != sound_dir.end (); sit++) {
            if ((*sit)[0] == '.')
                continue;
            SoundProfileImpl* entry;

            prof_path = path + (*sit) + "/";

            entry = new SoundProfileImpl ();
            entry->name = *sit;
            entry->directory = prof_path;

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
                            sound_count += load_sound (entry, EffectType (eti),
                                    prof_path + (*pit), data);
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

void AudioOpenAL::load_sounds () {
    vector<char> data;
    data.resize (0xffff);

    for (size_t di = 0; di < paths.get_data_dirs_count (); di++) {
        ustring dir = paths.get_data_dir (di) + "sounds/";
        scan_sounds_dir (dir, data);
    }
}

void AudioOpenAL::free_sounds () {
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

inline bool AudioOpenAL::music_is_playing () {
    ALenum state;
    alGetSourcei (music_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

AudioOpenAL::SoundProfileImpl& AudioOpenAL::get_sprofile (size_t index) {
    return *static_cast<SoundProfileImpl*> (sound_profiles[index]);
}

const AudioOpenAL::SoundProfileImpl& AudioOpenAL::get_sprofile (size_t index) const {
    return *static_cast<SoundProfileImpl*> (sound_profiles[index]);
}

bool AudioOpenAL::load_sound (SoundProfileImpl* prof, EffectType effect,
        const Glib::ustring& filename, vector<char>& data) const {
    MplayerDecoder dec;

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
        prof->buffers[effect].push_back (sound_buffer);
        return true;
    }
    return false;
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

inline AudioOpenAL& AudioOpenAL::get_instance () {
    return instance;
}

Audio& audio = AudioOpenAL::get_instance ();
