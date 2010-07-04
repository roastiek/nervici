#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "game/game.h"

typedef enum _EffectType {
    ET_Hop, ET_Jau, ET_Self, ET_SmilePlus, ET_SmileMinus, ET_Wall
} EffectType;

void audio_initialize ();

void audio_uninitialize ();

void audio_load_players (const GameInfo *info);

void audio_free_players ();

void audio_play_effect (int plid, EffectType effect);


typedef enum _MusicType {
    MT_Short, MT_Long, MT_Menu, MT_Stat, MT_Count
} MusicType;

inline _MusicType operator++(_MusicType &mt, int) {
    mt = (_MusicType) (mt + 1);
    return mt;
}

void music_play (MusicType type);

void music_stop ();

void music_update ();

void music_set_rate (float rate);


#endif // __AUDIO_H__
