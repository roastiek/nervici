#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "int_type.h"
#include "game/game_info.h"
#include "music_type.h"
#include "effect_type.h"

namespace Audio {
    void initialize ();

    void uninitialize ();

    void load_players (const GameInfo & info);

    void free_players ();

    void play_effect (plid_tu plid, EffectType effect);

    void music_play (MusicType type);

    void music_stop ();

    void music_update ();

    void music_set_rate (float rate);
};



#endif // __AUDIO_H__