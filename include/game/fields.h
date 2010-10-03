/* 
 * File:   fields.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 14:13
 * Used for calculation players bodies.
 */

#ifndef FIELDS_H
#define	FIELDS_H

#include "int_type.h"
#include "fakes/fpoint.h"

/*
 * A part of player's body is 2x2 pixels big, but because of that its position
 * is a float number, it is represented by 3x3 pixels matrix. The values (0-255) 
 * represent a density of body part in integers coordinates. They are used as 
 * alpha channel for drawing players. The center one is always 255.
 * Examples:
 * coords: (10, 10)
 *      255 255   0
 *      255 255   0
 *        0   0   0
 * coords: (10.5, 10.5)
 *      64  128  64
 *      128 255 127
 *      64  127  63
 */
struct Fields {
private:
    plpart_tu data[3][3];
public:
    template<class T>
    inline plpart_tu* operator[] (T index) {
        return data[index];
    }

    template<class T>
    inline const plpart_tu* operator[] (T index) const {
        return data[index];
    }

    /*
     * Update body part values for specifed position.
     */
    void calc (const FPoint& pos);

};

#endif	/* FIELDS_H */

