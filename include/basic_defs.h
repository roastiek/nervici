/* 
 * File:   basic_defs.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 13:17
 * 
 * This header contains basic constants and provides gettext itegration  
 */

#ifndef BASIC_DEFS_H
#define BASIC_DEFS_H

#include <libintl.h>

/*
 * Standards gettext macro
 */
#define _(str) (gettext(str))
/*
 * Used for marking static strings
 */
#define N_(str) (str)

/*
 * DECIMALS sets a precision of player's calculation. Numbers will have at a maximum as 
 * many decimals * as  1 / DECIMALS.
 */
#define DECIMALS 1000

/*
 * ANGLES represents how many times can player turn before he completes the whole 360 
 * cycle.  
 */
#define ANGLES 160

/*
 * BASE_SPEED sets basic delay between stesp of players in miliseconds.
 */
#define BASE_SPEED 12

/*
 * TEAMS_COUNT defines how many teams can be in the game. First one is always
 * hidden, so it should not be lower than 1.  
 */
#define TEAMS_COUNT 5

/*
 * JUMP_LENGTH specifies length of players'jump in steps.
 */
#define JUMP_LENGTH 24
/*
 * JUMP_REPEAT specifies time before player can jump again. It is counted from
 * beging of jump.
 */
#define JUMP_REPEAT 80

#endif	/* BASIC_DEFS_H */

