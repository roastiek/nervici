#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "game/game.h"

typedef enum EffectType {
    etHop, etJau, etSelf, etSmilePlus, etSmileMinus, etWall
} EffectType;

void audioInit ();
void audioUninit ();

void audioLoadPlayers (const GameInfo *info);
void audioFreePlayers ();

void audioPlayEffect (int plid, EffectType effect);


typedef enum MusicType {
    mtShort, mtLong, mtMenu, mtStat, mtCount
} MusicType;

void musicPlay (MusicType type);
void musicStop ();
void musicUpdate ();
void musicSetRate (float rate);


#endif // __AUDIO_H__
