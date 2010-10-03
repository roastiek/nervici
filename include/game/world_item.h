/* 
 * File:   world_item.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 14:12
 */

#ifndef WORLD_ITEM_H
#define	WORLD_ITEM_H

#include "int_type.h"

/*
 * Type of world item.
 */
enum ItemType {
    /*
     * Item is free. Can be occupied be player or smile.
     */
    IT_free,
    /*
     * Item belongs to a player.
     */
    IT_player,
    /*
     * Item is a stone. Not used now.
     */
    IT_stone,
    /*
     * Item is a wall. Used to make border around the playerground.
     */
    IT_wall,
    /*
     * Item is soft smile, can be eaten.
     */
    IT_soft_smile,
    /*
     * Item is hard smile, cannot be eaten.
     */
    IT_hard_smile
};

/*
 * Additional informations for item with a player.
 */
struct WIPlayer {
    /*
     * Player's id.
     */
    plid_tu ID;
    
    /*
     * Player's body density
     */
    plpart_tu body;
    
    /*
     * Index of the body part. Used to identify head and bottom.
     */
    plsize_tu order;
};

/*
 * Additional informations for item with a smile.
 */
struct WISmile {
    /*
     * Smile's id.
     */
    smileid_tu ID;
};

/*
 * Generic world item.
 */
struct WorldItem {
    /*
     * Type of world item.
     */
    ItemType type;
    
    /*
     * Item was changed from the last render.
     */
    bool changed;

    /*
     * Item specializations.
     */
    union {
        WIPlayer player;
        WISmile smile;
    };
};

#endif	/* WORLD_ITEM_H */

