/* 
 * File:   pl_state.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 13:56
 */

#ifndef PL_STATE_H
#define	PL_STATE_H

/*
 * PS_Start - player has got a start position and is ready to be started
 *            one dot is drawn
 * PS_Live  - player start moving from start
 * PS_Death - player has happily lived, but now is death, but his corpose remain
 * PS_Clear - player corpse start decaing from the oldest parts
 * PS_Erased- whole corse is gone
 * PS_Undeath - zombie state, player's corpose starts trying revive itself, when
 *              it is possible, player becomes live again.
 *
 * All players stats in state PS_Erased
 * PS_Erased -> PS_Start
 * PS_Start -> PS_Live
 * PS_Start -> PS_Death
 * PS_Start -> PS_Clear
 * PS_Live -> PS_Death
 * PS_Live -> PS_Clear
 * PS_Clear -> PS_Erased
 * PS_Clear -> PS_Undeath
 * PS_Death -> PS_Clear
 * PS_Death -> PS_Undeath
 * PS_Undeath -> PS_Clear
 * PS_Undeath -> PS_Live
 * PS_Undeath -> PS_Death
 */
typedef enum PlState {
    PS_Start, PS_Live, PS_Death, PS_Undeath, PS_Clear, PS_Erased, PS_Count
} PlState;

inline PlState operator ++(PlState& ps, int) {
    ps = (PlState) (ps + 1);
    return ps;
}



#endif	/* PL_STATE_H */

