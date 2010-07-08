#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "game/game_defs.h"

#include "audio_defs.h"

void audio_initialize ();

void audio_uninitialize ();

void audio_load_players (const GameInfo& info);

void audio_free_players ();

void audio_play_effect (int plid, EffectType effect);

void music_play (MusicType type);

void music_stop ();

void music_update ();

void music_set_rate (float rate);


#endif // __AUDIO_H__
