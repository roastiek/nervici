/* 
 * File:   smile.h
 * Author: bobo
 *
 * Created on 23. červenec 2010, 11:37
 */

#ifndef SMILE_H
#define	SMILE_H

#include "int_type.h"
#include "point.h"
#include "game/smyle_type.h"

class Smile {
protected:
    const smileid_tu id;
    const smilelvl_tu level;

    Point pos;

    bool visible;

    Smile (smileid_tu sid, smilelvl_tu lvl);

public:
    virtual ~Smile ();

    virtual void step () = 0;

    virtual void eat (plid_tu pid) = 0;

    virtual void erase ();

    virtual SmileType get_type () const = 0;

    smilelvl_tu get_level () const;
};

class SmileFactory {
public:
    static Smile* create (smileid_tu sid, smileid_tu order, smileid_tu count,
            SmileType stype, smilelvl_tu lvl);
};

#endif	/* SMILE_H */

