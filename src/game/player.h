#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "settings/plinfo.h"
#include "main.h"
#include "game/game.h"

typedef Uint8 Fields[3][3];

typedef enum PlState { 
    psStart, psLive, psDeath, psClear, psErased
} PlState;

typedef enum KeySt {
    ksNone, ksLeft, ksRight, ksJump, ksPower
} KeySt;

void players_initialize (const GameInfo& info);

void playersUninitialize ();
int playersStep ();
void  playersTimer (int speed);

int playerGetCount ();
int playerGetLivesCount ();
void playersClear ();

void clearPl (int plid);
void cutPlAtLength (int plid, int length);
void decPlMaxLength (int plid, unsigned int delta);
void decPlScore (int plid, int delta);
void fastClearPl (int plid);
int getPlLength (int plid);
int getPlMaxLength (int plid);
int getPlScore (int plid);
void givePlStart (int plid, int start);
void incPlMaxLength (int plid, unsigned int delta);
void incPlScore (int plid, int delta);
int isPlHuman (int plid);
int isPlJumping (int plid);
int isPlLive (int plid);
void killPl (int plid);
void setPlMaxLenght (int plid, unsigned int length);
void setPlScore (int plid, int score);
void setPlTimer (int plid, int time);
void startPl (int plid);
    
#endif
