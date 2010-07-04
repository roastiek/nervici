#include <al.h>
#include <alc.h>
#include <SDL.h>
#include <dirent.h>
#include <string.h>
#include <vorbisfile.h>

#include "audio.h"
#include "utils.h"
#include "system.h"
#include "settings/setting.h"

#define WAVS_COUNT 8
static const char *wavs[] = {
    "/hop.wav", "/jau1.wav", "/jau2.wav", "/self.wav", "/smileplus.wav",
    "/smileminus.wav", "/wall1.wav", "/wall2.wav"
};

typedef struct _SoundProfile {
    String name;
    String directory;
    ALuint buffers[WAVS_COUNT];
} SoundProfile;

typedef struct _SoundProfiles {
    size_t count;
    SoundProfile *items;
} SoundProfiles;

static const char *section = "audio";
static const char *st_sound = "sound";
static const char *st_music = "music";
static const char *st_buffer = "buffer";
static const char *st_bfrCount = "bfrCount";

typedef struct _AudioSetting {
    int sound;
    int music;
    int buffer;
    int bfrCount;
} AudioSetting;

static ALfloat sourcePos[] = {0.0, 0.0, 0.0};
static ALfloat sourceVel[] = {0.0, 0.0, 0.0};

static ALCdevice* pDevice;
static ALCcontext* pContext;
static ALfloat ListenerPos[] = {0.0, 0.0, 0.0};
static ALfloat ListenerVel[] = {0.0, 0.0, 0.0};
static ALfloat ListenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

static SoundProfiles sound_profiles;

static AudioSetting setting;

typedef struct _PlAudio {
    int prof;
    ALuint source;
    int r;
} PlAudio;

typedef struct _PlAudios {
    size_t count;
    PlAudio *items;
} PlAudios;

static PlAudios sources;

typedef struct _MusicFile {
    String filename;
    int played;
} MusicFile;

typedef struct _MusicFiles {
    size_t count;
    MusicFile *items;
} MusicFiles;

static MusicFiles music[MT_Count];

static void scan_sounds_dir (UnownedString path, SoundProfiles *profiles) {
    size_t path_len;
    int w;
    int valid;
    DIR *dir;
    struct dirent *ent;
    FILE *file;
    String prof_path;
    String wav_path;
    size_t prof_path_len;
    size_t wav_len;
    SoundProfile entry;

    dir = opendir (path);
    if (dir == NULL) return;

    path_len = strlen (path);

    for (ent = readdir (dir); ent != NULL; ent = readdir (dir)) {
        if (ent->d_name[0] == '.') continue;

        prof_path = str_concat_path (path, ent->d_name);
        wav_path = str_bigger_copy (prof_path, 16);
        prof_path_len = strlen (prof_path);

        valid = 1;
        for (w = 0; w < WAVS_COUNT; w++) {
            wav_len = strlen (wavs[w]);
            memcpy (&wav_path[prof_path_len], wavs[w], wav_len + 1);

            file = fopen (wav_path, "rb");
            if (file == NULL) {
                valid = 0;
                break;
            }
            fclose (file);
        }
        free (wav_path);

        if (valid) {
            memset (&entry, 0, sizeof (entry));
            entry.directory = prof_path;
            entry.name = str_copy (ent->d_name);

            array_append (*profiles, entry);
        } else {
            free (prof_path);
        }
    }

    closedir (dir);
}

static ALuint load_wav (UnownedString filename) {
    SDL_AudioSpec spec;
    Uint8 *data;
    Uint32 len;
    ALuint result;

    alGenBuffers (1, &result);

    if (SDL_LoadWAV (filename, &spec, &data, &len) == NULL) return result;
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

static void load_buffers (SoundProfiles *profiles) {
    int w;
    size_t dir_len;
    char *filename;
    SoundProfile *spi;

    for (int pi = 0; pi < profiles->count; pi++) {
        spi = &profiles->items[pi];

        dir_len = strlen (spi->directory);
        filename = str_bigger_copy (spi->directory, 16);

        for (w = 0; w < WAVS_COUNT; w++) {
            memcpy (&filename[dir_len], wavs[w], strlen (wavs[w] + 1));
            spi->buffers[w] = load_wav (filename);
        }

        free (filename);
    }
}

static void load_wavs () {
    printf ("loadWavs\n");

    sound_profiles.count = 0;
    sound_profiles.items = NULL;
    scan_sounds_dir (sys_get_sounds_dir (), &sound_profiles);
    scan_sounds_dir (sys_get_sounds_dir_home (), &sound_profiles);

    load_buffers (&sound_profiles);
}

static void free_wavs () {
    SoundProfile *spi;

    for (int pi = 0; pi < sound_profiles.count; pi++) {
        spi = &sound_profiles.items[pi];
        free (spi->name);
        free (spi->directory);
        alDeleteBuffers (WAVS_COUNT, spi->buffers);
    }
    sound_profiles.count = 0;
    free (sound_profiles.items);
}

static int ogg_get_length (UnownedString filename) {
    OggVorbis_File file;
    int result;
    size_t len;

    len = strlen (filename);
    char fn[len + 1];
    memcpy (fn, filename, len + 1);

    if (ov_fopen (fn, &file)) return -1;

    result = ov_time_total (&file, -1);
    ov_clear (&file);

    return result;
}

static void scan_music_dir (UnownedString path, MusicFiles *files, MusicType type) {
    DIR *dir;
    size_t path_len;
    struct dirent *ent;
    int len;
    String file_path;
    MusicFile entry;

    dir = opendir (path);
    if (dir == NULL) return;

    path_len = strlen (path);

    for (ent = readdir (dir); ent != NULL; ent = readdir (dir)) {
        if (ent->d_name[0] == '.') continue;


        file_path = str_concat_path (path, ent->d_name);

        len = ogg_get_length (file_path);

        if (len <= 0) continue;
        if (type == MT_Short && len > 150) {
            free (file_path);
            continue;
        }
        if (type == MT_Long && len <= 150) {
            free (file_path);
            continue;
        }

        entry.filename = file_path;
        entry.played = 0;

        array_append (*files, entry);
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
static int musicLoop = 0;
static int musicOpened = 0;

static void load_music () {
    static const char suffixs[MT_Count][6] = {"/game", "/game", "/menu", "/stat"};
    char *dir_name;
    size_t dir_len;
    MusicType mt;

    printf ("loadMusic\n");

    for (mt = MT_Short; mt < MT_Count; mt++) {
        music[mt].count = 0;
        music[mt].items = NULL;
    }

    dir_name = str_bigger_copy (sys_get_music_dir (), 8);
    dir_len = strlen (dir_name);

    for (mt = MT_Short; mt < MT_Count; mt++) {
        memcpy (&dir_name[dir_len], suffixs[mt], strlen (suffixs[mt]) + 1);
        scan_music_dir (dir_name, &music[mt], mt);
    }

    free (dir_name);

    dir_name = str_bigger_copy (sys_get_music_dir_home (), 8);
    dir_len = strlen (dir_name);

    for (mt = MT_Short; mt < MT_Count; mt++) {
        memcpy (&dir_name[dir_len], suffixs[mt], strlen (suffixs[mt]) + 1);
        scan_music_dir (dir_name, &music[mt], mt);
    }

    free (dir_name);

    mloader = malloc (setting.buffer);
    musicBuffers = malloc (sizeof (ALuint) * setting.bfrCount);

    alGenBuffers (setting.bfrCount, musicBuffers);
    alGenSources (1, &musicSource);

    alSourcefv (musicSource, AL_POSITION, sourcePos);
    alSourcefv (musicSource, AL_VELOCITY, sourceVel);
    alSourcef (musicSource, AL_GAIN, setting.music / 20.0);

}

static void free_music () {
    MusicType mt;
    int mi;

    for (mt = MT_Short; mt < MT_Count; mt++) {
        for (mi = 0; mi < music[mt].count; mi++) {
            free (music[mt].items[mi].filename);
        }
        free (music[mt].items);
    }

    alDeleteBuffers (setting.bfrCount, musicBuffers);
    alDeleteSources (1, &musicSource);

    free (mloader);
    free (musicBuffers);
}

static void load_audio_setting () {
    printf ("loadAudioSetting\n");

    setting.sound = setting_read_int (section, st_sound, 20);
    setting.music = setting_read_int (section, st_music, 20);
    setting.buffer = setting_read_int (section, st_buffer, 0x10000);
    setting.bfrCount = setting_read_int (section, st_bfrCount, 2);
}

static void save_audio_setting () {
    setting_write_int (section, st_sound, setting.sound);
    setting_write_int (section, st_music, setting.music);
    setting_write_int (section, st_buffer, setting.buffer);
    setting_write_int (section, st_bfrCount, setting.bfrCount);
}

void audio_initialize () {
    printf ("audioInit\n");
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

static int find_profil (const char *name) {
    int si;

    for (si = 0; si < sound_profiles.count; si++) {
        if (strcasecmp (name, sound_profiles.items[si].name) == 0) {
            return si;
        }
    }
    return -1;
}

void audio_load_players (const GameInfo *info) {
    int si;

    sources.count = info->plsCount;
    sources.items = malloc (sizeof (PlAudio) * sources.count);

    for (si = 0; si < sources.count; si++) {
        alGenSources (1, &sources.items[si].source);
        alSourcef (sources.items[si].source, AL_PITCH, (info->plInfos[si].pitch + 5) / 10.0);
        alSourcef (sources.items[si].source, AL_GAIN, setting.sound / 20.0);
        alSourcefv (sources.items[si].source, AL_VELOCITY, sourceVel);
        alSourcefv (sources.items[si].source, AL_POSITION, sourcePos);
        sources.items[si].prof = find_profil (info->plInfos[si].profil);
    }
}

void audio_free_players () {
    int si;

    for (si = 0; si < sources.count; si++) {
        alSourceStop (sources.items[si].source);
        alDeleteSources (1, &sources.items[si].source);
    }
}

void audio_play_effect (int plid, EffectType effect) {
    ALint play;
    SoundProfile *spi;
    ALuint source;

    if (sources.items[plid].prof >= 0) {
        alGetSourcei (sources.items[plid].source, AL_SOURCE_STATE, &play);

        spi = &sound_profiles.items[sources.items[plid].prof];
        source = sources.items[plid].source;

        if (play != AL_PLAYING) {
            switch (effect) {
                case ET_Hop:
                    alSourcei (source, AL_BUFFER, spi->buffers[0]);
                    break;
                case ET_Jau:
                    alSourcei (source, AL_BUFFER, spi->buffers[1 + (sources.items[plid].r % 2)]);
                    sources.items[plid].r++;
                    break;
                case ET_Self:
                    alSourcei (source, AL_BUFFER, spi->buffers[3]);
                    break;
                case ET_SmilePlus:
                    alSourcei (source, AL_BUFFER, spi->buffers[4]);
                    break;
                case ET_SmileMinus:
                    alSourcei (source, AL_BUFFER, spi->buffers[5]);
                    break;
                case ET_Wall:
                    alSourcei (source, AL_BUFFER, spi->buffers[6 + (sources.items[plid].r % 2)]);
                    sources.items[plid].r++;
                    break;
            }

            alSourcePlay (source);
        }
    }
}

static int music_open (MusicType type) {
    int f, count;
    int op;
    int mi;

    count = 0;
    for (mi = 0; mi < music[type].count; mi++) {
        count += music[type].items[mi].played & 1;
    }

    if (count == music[type].count) {
        for (mi = 0; mi < music[type].count; mi++) {
            music[type].items[mi].played = 0;
        }
        count = 0;
    }

    while (count < music[type].count) {
        printf ("music open %d/%ld\n", count, music[type].count);
        f = rand () % music[type].count;
        if (!music[type].items[f].played) {
            music[type].items[f].played = 1;
            count++;

            op = ov_fopen (music[type].items[f].filename, &musicFile);
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

            musicOpened = 1;
            return 0;
        }
    }

    return -1;
}

static void music_close () {
    if (musicOpened) {
        ov_clear (&musicFile);
        musicOpened = 0;
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
    musicLoop = 1;
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
    musicLoop = 0;
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
