#ifndef __WORLD_H__
#define __WORLD_H__

#define IT_FREE         0
#define IT_PLAYER       1
#define IT_STONE        2
#define IT_WALL         3
#define IT_SOFT_SMILE   4
#define IT_HARD_SMILE   5

#include "main.h"

typedef struct WIPlayer {
    Uint8 type;
    Uint8 ID;
    Uint8 body;
    Uint16 order;
} WIPlayer;

typedef struct WISmile {
    Uint8 type;
    Uint32 ID;
} WISmile;

typedef union WorldItem {
    Uint8 type;
    WIPlayer player;
    WISmile smile;
} WorldItem;

typedef struct Start {
    FPoint pos;
    int angle;
    int ready;
} Start;

void worldInitialize (WorldItem *items);
void worldUninitialize ();
void worldClear ();

int getWorldWidth ();
int getWorldHeight ();
WorldItem *getWorldItem (int x, int y);
WorldItem *getWorldItemp (Point16 pos);

void checkStarts ();
const Start *getStart (int stid);
int getStartsCount ();
int findFreeStart ();

#endif
