#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nervici.h"
#include "mods.h"

static ModInfo info = {
    "cervici",
    "bobo",
    "proste cervici"
};

static const GameSetting *set;
static int delta = -1;

const ModInfo *getModInfo () {
    return &info;
}

void onGameStart (const GameSetting *nset) {
    int p, s;

    set = nset;
    
    for (p = 0; p < set->playersCount; p++) {
        printf ("game start %d\n", p);
        s = findFreeStart ();
        if (s != -1) {
            givePlStart (p, s);
            setPlTimer (p, -1000);
        }
    }
    printf ("game start\n");
    
    setTimer (-3000);
}

void onGameEnd () {
}

void onTimer () {
}

void onDeath (int plid) {
}

void beforeStep () {
}

void afterStep () {
    int p;
    
    for (p = 0; p < set->playersCount; p++) {
        if (isPlLive (p)) {
            incPlScore (p, 1);
        }
    }
}

void onPoziSmile (int smid, int lvl) {
}

void onNegaSmile (int smid, int lvl) {
}

void onFlegSmile (int smid, int lvl) {
}

void onIronSmile (int smid, int lvl) {
}

void onHamSmile (int smid, int lvl) {
}

void onKilled (int plid, int murder) {
}

void onKill (int plid,int victim) {
}

void onWall (int plid) {
}

void onSelfDeath (int plid) {
}

void onCleared (int plid) {
}

void onPlTimer (int plid) {
    startPl (plid);
    playMusic (0);
    setSemafor (1);
    nextRound ();
}

