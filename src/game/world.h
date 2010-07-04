#ifndef __WORLD_H__
#define __WORLD_H__

#define IT_FREE         0
#define IT_PLAYER       1
#define IT_STONE        2
#define IT_WALL         3
#define IT_SOFT_SMILE   4
#define IT_HARD_SMILE   5

#include "main.h"

typedef struct _WIPlayer {
    Uint8 type;
    Uint8 ID;
    Uint8 body;
    Uint16 order;
} WIPlayer;

typedef struct _WISmile {
    Uint8 type;
    Uint32 ID;
} WISmile;

typedef union _WorldItem {
    Uint8 type;
    WIPlayer player;
    WISmile smile;
} WorldItem;

typedef struct _Start {
    FPoint pos;
    int angle;
    int ready;
} Start;

void world_initialize ();

void world_uninitialize ();

void world_clear ();

int world_get_width ();

int world_get_height ();

WorldItem *world_get_item (int x, int y);

WorldItem *world_get_item_p (Point16 pos);

void world_check_starts ();

const Start *world_get_start (int stid);

int world_get_starts_count ();

int world_find_free_start ();

#endif
