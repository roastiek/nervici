/* 
 * File:   world_item.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 14:12
 */

#ifndef WORLD_ITEM_H
#define	WORLD_ITEM_H

#include "int_type.h"

#define IT_FREE         0
#define IT_PLAYER       1
#define IT_STONE        3
#define IT_WALL         4
#define IT_SOFT_SMILE   5
#define IT_HARD_SMILE   6

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

#endif	/* WORLD_ITEM_H */

