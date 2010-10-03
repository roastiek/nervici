/* 
 * File:   death_cause.h
 * Author: deedrah
 *
 * Created on 17. srpen 2010, 13:02
 * Information about a death cause of player.
 */

#ifndef DEATH_CAUSE_H
#define	DEATH_CAUSE_H

#include "int_type.h"

/*
 * Types of all possible deaths.
 */
enum DeathCauses {
    /*
     * Player will not die.
     */
    DC_none,
    /*
     * Player has crashed into a wall.
     */
    DC_wall,
    /*
     * Player has been killed by another player.
     */
    DC_killed,
    /*
     * Player has commited suicide.
     */
    DC_self,
    /*
     * Player has crashed int a hard smile.
     */
    DC_smile
};

/*
 * Contains type of the death and additiona informations for some causes.
 * For DC_killed, murded option is set.
 * For DC_smile, smile option is set.
 */
struct DeathCause {
public:
    DeathCauses cause;
    union {
        plid_tu murder;
        smileid_tu smile;
    };
};

#endif	/* DEATH_CAUSE_H */

