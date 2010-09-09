/* 
 * File:   world_item.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 14:12
 */

#ifndef WORLD_ITEM_H
#define	WORLD_ITEM_H

#include "int_type.h"

/*#define IT_FREE         0
#define IT_PLAYER       1
#define IT_STONE        3
#define IT_WALL         4
#define IT_SOFT_SMILE   5
#define IT_HARD_SMILE   6*/

enum ItemType {
    IT_free, IT_player, IT_stone, IT_wall, IT_soft_smile, IT_hard_smile
};

struct WIPlayer {
    plid_tu ID;
    plpart_tu body;
    plsize_tu order;
};

struct WISmile {
    smileid_tu ID;
};

struct WorldItem {
    ItemType type;
    bool changed;

    union {
        WIPlayer player;
        WISmile smile;
    };
};

#endif	/* WORLD_ITEM_H */

