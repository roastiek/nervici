/* 
 * File:   death_cause.h
 * Author: bobo
 *
 * Created on 17. srpen 2010, 13:02
 */

#ifndef DEATH_CAUSE_H
#define	DEATH_CAUSE_H

#include "int_type.h"

enum DeathCauses {
    DC_none, DC_wall, DC_killed, DC_self, DC_smile
};

struct DeathCause {
public:
    DeathCauses cause;
    union {
        plid_tu murder;
        smileid_tu smile;
    };
};

#endif	/* DEATH_CAUSE_H */

