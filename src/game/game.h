#ifndef __GAME_H__
#define __GAME_H__

#include "mods/mods.h"
#include "settings/plinfo.h"
#include "world.h"

typedef struct _GameInfo {
    GameSetting *setting;
    int plsCount;
    PlInfo *plInfos;
} GameInfo;

void gameInitialize (const GameInfo& info);
void gameRun ();
void gameUninitialize ();

void clearPlayerground ();
void clearStatus ();
void endGame ();
int getSpeed ();
int livePlsCount ();
void nextRound ();
void playMusic (int type);
int getRound ();
void setSemafor (int state);
void setSpeed (int speed);
void setTimer (int time);
void stopMusic ();
void gameWait (int time);
void waitForSpace ();

#endif
