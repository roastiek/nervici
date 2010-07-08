#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <vorbisfile.h>
#include <alc.h>

#include "game/game_defs.h"

#include "audio_defs.h"

struct Audio {
private:

    static float music_rate;

    static bool music_loop;

    static bool music_opened;

    static OggVorbis_File music_file;

    static vorbis_info *music_info;

    static ALuint *music_buffers;

    static ALuint music_source;

    static ALenum music_format;

    static MusicType music_type;

    static char *music_loader;

    static ALCdevice* device;

    static ALCcontext* context;

    static AudioSetting setting;

    static vector<SoundProfile> sound_profiles;

    static vector<PlAudio> sounds;

    static vector<MusicFile> music[MT_Count];

    static int music_stream (ALuint buffer);

    static bool music_open (MusicType type);

    static void music_close ();

    static void init_music ();

    static void free_music ();

    static void init_wavs ();

    static void free_wavs ();

    static void load_setting ();

    static void save_setting ();

    static int find_profil (const string& name);

public:
    static void initialize ();

    static void uninitialize ();

    static void load_players (const GameInfo & info);

    static void free_players ();

    static void play_effect (plid_tu plid, EffectType effect);

    static void music_play (MusicType type);

    static void music_stop ();

    static void music_update ();

    static void music_set_rate (float rate) {
        music_rate = rate;
    }
};



#endif // __AUDIO_H__
