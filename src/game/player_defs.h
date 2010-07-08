/* 
 * File:   player_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 17:21
 */

#ifndef PLAYER_DEFS_H
#define	PLAYER_DEFS_H

#define JUMP_LENGTH 24
#define JUMP_REPEAT 80

/*
 * PS_Start - player has got a start position and is ready to be started
 *            one dot is drawn
 * PS_Live  - player start moving from start
 * PS_Death - player has happily lived, but now is death, but his corpose remain
 * PS_Clear - player corpse start decaing from the oldest parts
 * PS_Erased- whole corse is gone
 *
 * All players stats in state PS_Erased
 * PS_Erased -> PS_Start
 * PS_Start -> PS_Live
 * PS_Start -> PS_Death
 * PS_Start -> PS_Clear
 * PS_Live -> PS_Death
 * PS_Live -> PS_Clear
 * PS_Clear -> PS_Erased
 */
typedef enum PlState {
    PS_Start, PS_Live, PS_Death, PS_Clear, PS_Erased
} PlState;

typedef enum KeySt {
    KS_None, KS_Left, KS_Right, KS_Jump, KS_Power
} KeySt;

#endif	/* PLAYER_DEFS_H */

