/* 
 * File:   start.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 14:12
 */

#ifndef START_H
#define	START_H

#include "fpoint.h"

struct Start {
    FPoint pos;
    uint8_t angle;
    bool ready;
};

#endif	/* START_H */

