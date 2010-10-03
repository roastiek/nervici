/* 
 * File:   pl_state.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 13:56
 * Describes all states, that can a player went through in a game.
 */

#ifndef PL_STATE_H
#define	PL_STATE_H

/* All players stats in state PS_Erased
 * Possible switches between states:
 * PS_Erased  -> PS_Start
 * PS_Start   -> PS_Live
 * PS_Start   -> PS_Death
 * PS_Start   -> PS_Clear
 * PS_Live    -> PS_Death
 * PS_Live    -> PS_Clear
 * PS_Clear   -> PS_Erased
 * PS_Clear   -> PS_Undeath
 * PS_Death   -> PS_Clear
 * PS_Death   -> PS_Undeath
 * PS_Undeath -> PS_Clear
 * PS_Undeath -> PS_Live
 * PS_Undeath -> PS_Death
 */
typedef enum PlState {
    /*
     * Player has got a start position and is ready to be started. His first
     * body part has been drawn in the start position.
     */
    PS_Start,

    /* 
     * Player is moving on the playground and trying to gain some points
     */
    PS_Live,

    /* 
     * Player has happily lived, but now he is death, but his corpose still 
     * remains.
     */
    PS_Death,

    /*
     * Zombie state. Player's corpose keeps trying to revive itself. When it is 
     * possible (the obstuction has been cleared off), player becomes living 
     * again.
     */
    PS_Undeath,

    /*
     * Player's corpse is decaing from the oldest parts.
     */
    PS_Clear,

    /*
     * Whole corse is gone, has decaied. The player is not visible on 
     * playerground.
     */
    PS_Erased,

    /*
     * Count of states.
     */
    PS_Count
} PlState;

#endif	/* PL_STATE_H */

