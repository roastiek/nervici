/* 
 * File:   world_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 17:28
 */

#ifndef WORLD_DEFS_H
#define	WORLD_DEFS_H

#include "int_type.h"

#define IT_FREE         0
#define IT_PLAYER       1
#define IT_STONE        2
#define IT_WALL         3
#define IT_SOFT_SMILE   4
#define IT_HARD_SMILE   5

typedef plpart_tu Fields[3][3];

struct WIPlayer {
    plid_tu ID;
    plpart_tu body;
    plsize_tu order;
};

struct WISmile {
    uint32_t ID;
};

struct WorldItem {
    uint8_t type;
    bool changed;

    union {
        WIPlayer player;
        WISmile smile;
    };
};

struct Start {
    FPoint pos;
    uint8_t angle;
    bool ready;
};

#endif	/* WORLD_DEFS_H */

