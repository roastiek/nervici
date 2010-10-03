/* 
 * File:   key_state.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 13:57
 */

#ifndef KEY_STATE_H
#define	KEY_STATE_H

/*
 * State of pressed keys by player. Used for generelization input of human and 
 * ai players.
 */
typedef enum KeySt {
    /*
     * No key pressed.
     */
    KS_None,
    /*
     * Only "left" key pressed. 
     */
    KS_Left,
    /*
     * Only "right" key pressed.
     */
    KS_Right,
    /*
     * "Jump" key preseed.
     */
    KS_Jump,
    /*
     * Both "left" and "right" keys pressed and same time. Not used now.
     */
    KS_Power
} KeySt;

#endif	/* KEY_STATE_H */

