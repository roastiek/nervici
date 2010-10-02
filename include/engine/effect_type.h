/* 
 * File:   effect_type.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 15:51
 */

#ifndef EFFECT_TYPE_H
#define	EFFECT_TYPE_H

/*
 * Represents sound effects for players actions.
 */
enum EffectType {
    /*
     * Player has jumped.
     */
    ET_Jump, 
    /*
     * Player has bumped into another.
     */
    ET_Au,
    /*
     * Player has bimped into himsefl.
     */
    ET_Self,
    /*
     * Player has eaten good smile.
     */
    ET_SmilePlus,
    /*
     * Player has eaten bad smile.
     */
    ET_SmileMinus, 
    /*
     * Player has crshed into a wall.
     */
    ET_Wall,
    /*
     * Count of effects.
     */
    ET_Count
};

#endif	/* EFFECT_TYPE_H */

