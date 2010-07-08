#include <al.h>
#include <alc.h>
#include <SDL.h>
#include <dirent.h>
//#include <string.h>
#include <vorbisfile.h>
#include <AL/al.h>
#include <vector>
#include <iostream>

#include "system.h"
#include "settings/setting.h"

#include "audio.h"

#define WAVS_COUNT 8
static const char* const wavs[] = {
    "/hop.wav", "/jau1.wav", "/jau2.wav", "/self.wav", "/smileplus.wav",
    "/smileminus.wav", "/wall1.wav", "/wall2.wav"
};

static const char* const section = "audio";
static const char* const st_sound = "sound";
static const char* const st_music = "music";
static const char* const st_buffer = "buffer";
static const char* const st_bfrCount = "bfrCount";

struct SoundProfile {
    string name;
    string directory;
    ALuint buffers[WAVS_COUNT];
};

typedef vector<SoundProfile> SoundProfiles;

struct AudioSetting {
    int sound;
    int music;
    int buffer;
    int bfrCount;
};

struct PlAudio {
    int prof;
    ALuint source;
    int r;
};

typedef vector<PlAudio> PlAudios;

struct MusicFile {
    string filename;
    int played;
};

typedef vector<MusicFile> MusicFiles;

static ALfloat sourcePos[] = {0.0, 0.0, 0.0};
static ALfloat sourceVel[] = {0.0, 0.0, 0.0};

static ALCdevice* pDevice;
static ALCcontext* pContext;
static ALfloat ListenerPos[] = {0.0, 0.0, 0.0};
static ALfloat ListenerVel[] = {0.0, 0.0, 0.0};
static ALfloat ListenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

static SoundProfiles sound_profiles;

static AudioSetting setting;

static PlAudios sources;

static MusicFiles music[MT_Count];

static void scan_sounds_dir (const string& path, SoundProfiles& profiles) {
    int w;
    int valid;
    DIR *dir;
    struct dirent *ent;
    FILE *file;
    string prof_path;
    string wav_path;
    SoundProfile entry;

    dir = opendir (path.c_str());
    if (dir == NULL) return;

    for (ent = readdir (dir); ent != NULL; ent = readdir (dir)) {
        if (ent->d_name[0] == '.') continue;

        prof_path = path + '/' + ent->d_name;

        valid = 1;
        for (w = 0; w < WAVS_COUNT; w++) {
            wav_path = prof_path + wavs[w];

            file = fopen (wav_path.c_str(), "rb");
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

static ALuint load_wav (const string& filename) {
    SDL_AudioSpec spec;
    Uint8 *data;
    Uint32 len;
    ALuint result;

    alGenBuffers (1, &result);

    if (SDL_LoadWAV (filename.c_str(), &spec, &data, &len) == NULL) return result;
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

static void load_buffers (SoundProfiles& profiles) {
    int w;
    string filename;

    for (size_t pi = 0; pi < profiles.size(); pi++) {
        SoundProfile& spi = profiles[pi];

        for (w = 0; w < WAVS_COUNT; w++) {
            filename = spi.directory + wavs[w];
            spi.buffers[w] = load_wav (filename);
        }
    }
}

static void load_wavs () {
    cout << __func__ << '\n';

    scan_sounds_dir (System::get_sounds_dir (), sound_profiles);
    scan_sounds_dir (System::get_sounds_dir_home (), sound_profiles);

    load_buffers (sound_profiles);
}

static void free_wavs () {
    for (size_t pi = 0; pi < sound_profiles.size (); pi++) {
        SoundProfile& spi = sound_profiles[pi];
        alDeleteBuffers (WAVS_COUNT, spi.buffers);
    }
}

static double ogg_get_length (const string& filename) {
    OggVorbis_File file;
    double result;
    size_t len;
    char *fn;

    len = filename.length();
    fn = (char *) alloca(len + 1);
    memcpy (fn, filename.c_str(), len + 1);

    if (ov_fopen (fn, &file)) return -1;

    result = ov_time_total (&file, -1);
    ov_clear (&file);

    return result;
}

static void scan_music_dir (const string& path, MusicFiles& files, MusicType type) {
    DIR *dir;
    struct dirent *ent;
    double len;
    string file_path;
    MusicFile entry;

    dir = opendir (path.c_str());
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
        entry.played = 0;
        files.push_back(entry);
    }

    closedir (dir);
}

static OggVorbis_File musicFile;
static vorbis_info *musicInfo;
static ALuint *musicBuffers;
static ALuint musicSource;
static ALenum musicFormat;
static float musicRate = 1.0;
static MusicType musicType = MT_Count;
static char *mloader = NULL;
static bool musicLoop = false;
static bool musicOpened = false;

static void load_music () {
    static const char* const suffixs[MT_Count] = {"/game", "/game", "/menu", "/stat"};
    string dir_name;
    MusicType mt;

    cout << __func__ << '\n';

    for (mt = MT_Short; mt < MT_Count; mt++) {
        dir_name = System::get_music_dir() + suffixs[mt];
        scan_music_dir (dir_name, music[mt], mt);
    }

    for (mt = MT_Short; mt < MT_Count; mt++) {
        dir_name = System::get_music_dir_home() + suffixs[mt];
        scan_music_dir (dir_name, music[mt], mt);
    }

    mloader = new char[setting.buffer];
    musicBuffers = new ALuint[setting.bfrCount];

    alGenBuffers (setting.bfrCount, musicBuffers);
    alGenSources (1, &musicSource);

    alSourcefv (musicSource, AL_POSITION, sourcePos);
    alSourcefv (musicSource, AL_VELOCITY, sourceVel);
    alSourcef (musicSource, AL_GAIN, setting.music / 20.0);
}

static void free_music () {
    alDeleteBuffers (setting.bfrCount, musicBuffers);
    alDeleteSources (1, &musicSource);

    delete [] mloader;
    delete [] musicBuffers;
}

static void load_audio_setting () {
    cout << __func__ << '\n';

    setting.sound = Setting::read_int (section, st_sound, 20);
    setting.music = Setting::read_int (section, st_music, 20);
    setting.buffer = Setting::read_int (section, st_buffer, 0x10000);
    setting.bfrCount = Setting::read_int (section, st_bfrCount, 2);
}

static void save_audio_setting () {
    Setting::write_int (section, st_sound, setting.sound);
    Setting::write_int (section, st_music, setting.music);
    Setting::write_int (section, st_buffer, setting.buffer);
    Setting::write_int (section, st_bfrCount, setting.bfrCount);
}

void audio_initialize () {
    cout << __func__ << '\n';
    pDevice = alcOpenDevice (NULL);
    pContext = alcCreateContext (pDevice, NULL);
    alcMakeContextCurrent (pContext);

    alListenerfv (AL_POSITION, ListenerPos);
    alListenerfv (AL_VELOCITY, ListenerVel);
    alListenerfv (AL_ORIENTATION, ListenerOri);

    load_audio_setting ();

    load_wavs ();
    load_music ();
}

void audio_uninitialize () {
    save_audio_setting ();

    free_music ();
    free_wavs ();

    alcMakeContextCurrent (NULL);
    alcDestroyContext (pContext);
    alcCloseDevice (pDevice);
}

static int find_profil (string name) {
    for (size_t si = 0; si < sound_profiles.size (); si++) {
        if (strcasecmp (name.c_str(), sound_profiles[si].name.c_str()) == 0) {
            return si;
        }
    }
    return -1;
}

void audio_load_players (const GameInfo& info) {
    sources.clear();

    for (size_t si = 0; si < info.pl_infos.size (); si++) {
        PlAudio entry;
        alGenSources (1, &entry.source);
        alSourcef (entry.source, AL_PITCH, (info.pl_infos[si].pitch + 5) / 10.0);
        alSourcef (entry.source, AL_GAIN, setting.sound / 20.0);
        alSourcefv (entry.source, AL_VELOCITY, sourceVel);
        alSourcefv (entry.source, AL_POSITION, sourcePos);
        entry.prof = find_profil (info.pl_infos[si].profil);
        sources.push_back(entry);
    }
}

void audio_free_players () {
    for (size_t si = 0; si < sources.size (); si++) {
        alSourceStop (sources[si].source);
        alDeleteSources (1, &sources[si].source);
    }
}

void audio_play_effect (int plid, EffectType effect) {
    ALint play;
    //SoundProfile *spi;
    ALuint source;

    if (sources[plid].prof >= 0) {
        alGetSourcei (sources[plid].source, AL_SOURCE_STATE, &play);

        SoundProfile& spi = sound_profiles[sources[plid].prof];
        source = sources[plid].source;

        if (play != AL_PLAYING) {
            switch (effect) {
                case ET_Hop:
                    alSourcei (source, AL_BUFFER, spi.buffers[0]);
                    break;
                case ET_Jau:
                    alSourcei (source, AL_BUFFER, spi.buffers[1 + (sources[plid].r % 2)]);
                    sources[plid].r++;
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
                    alSourcei (source, AL_BUFFER, spi.buffers[6 + (sources[plid].r % 2)]);
                    sources[plid].r++;
                    break;
            }

            alSourcePlay (source);
        }
    }
}

static int music_open (MusicType type) {
    int f;
    int op;
    size_t count;

    count = 0;
    for (size_t mi = 0; mi < music[type].size (); mi++) {
        count += music[type][mi].played & 1;
    }

    if (count == music[type].size ()) {
        for (size_t mi = 0; mi < music[type].size (); mi++) {
            music[type][mi].played = 0;
        }
        count = 0;
    }

    while (count < music[type].size ()) {
        cout << "music open " << count << '/' << music[type].size () << '\n';
        f = rand () % music[type].size ();
        if (!music[type][f].played) {
            music[type][f].played = 1;
            count++;

            op = ov_fopen ((char *)music[type][f].filename.c_str(), &musicFile);
            if (op) continue;

            musicInfo = ov_info (&musicFile, -1);

            if (musicInfo->channels == 1)
                musicFormat = AL_FORMAT_MONO16;
            else if (musicInfo->channels == 2)
                musicFormat = AL_FORMAT_STEREO16;
            else {
                ov_clear (&musicFile);
                continue;
            }

            musicOpened = true;
            return 0;
        }
    }

    return -1;
}

static void music_close () {
    if (musicOpened) {
        ov_clear (&musicFile);
        musicOpened = false;
    }
}

static int music_stream (ALuint buffer) {
    int size = 0;
    int section;
    int result;
    int bs;

    bs = setting.buffer / 2;
    while (bs - 1024 >= (setting.buffer / 2) * musicRate) bs -= 1024;
    while (bs < (setting.buffer / 2) * musicRate) bs += 1024;

    while (size < bs) {
        result = ov_read (&musicFile, mloader + size, bs - size, 0, 2, 1, &section);

        if (result > 0) size += result;
        else return result;
    }

    alBufferData (buffer, musicFormat, mloader, size, musicInfo->rate * musicRate);
    return size;
}

void music_play (MusicType type) {
    ALenum state;
    int b;

    alGetSourcei (musicSource, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        if (musicType == type) return;
    }

    music_stop ();

    musicType = type;
    if (music_open (type)) return;

    for (b = 0; b < setting.bfrCount; b++) {
        if (music_stream (musicBuffers[b]) <= 0) {
            break;
        }
        alSourceQueueBuffers (musicSource, 1, &musicBuffers[b]);
    }

    alSourcePlay (musicSource);
    musicLoop = true;
}

void music_stop () {
    int queued;
    ALuint buffer;

    alSourceStop (musicSource);
    alGetSourcei (musicSource, AL_BUFFERS_QUEUED, &queued);

    for (; queued > 0; queued--) {
        alSourceUnqueueBuffers (musicSource, 1, &buffer);
    }

    music_close ();
    musicLoop = false;
}

void music_update () {
    int processed;
    ALenum state;
    ALuint buffer;

    alGetSourcei (musicSource, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {

        alGetSourcei (musicSource, AL_BUFFERS_PROCESSED, &processed);

        for (; processed > 0; processed--) {
            alSourceUnqueueBuffers (musicSource, 1, &buffer);
            if (music_stream (buffer) > 0)
                alSourceQueueBuffers (musicSource, 1, &buffer);
        }
    } else if (musicLoop) {
        music_play (musicType);
    }

}

void music_set_rate (float rate) {
    musicRate = rate;
}
