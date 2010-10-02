/* 
 * File:   render_semafor.h
 * Author: deedrah
 *
 * Created on 8. červenec 2010, 17:37
 */

#ifndef RENDER_SEMAFOR_H
#define	RENDER_SEMAFOR_H

/*
 * Represents simple states of a semafor, they can be combined together.
 */
enum Semafor {
    SEMAFOR_OFF = 0x0,
    SEMAFOR_RED_1 = 0x1,
    SEMAFOR_RED_2 = 0x2,
    SEMAFOR_RED_3 = 0x4,
    SEMAFOR_GREEN = 0x8
};

#endif	/* RENDER_SEMAFOR_H */

