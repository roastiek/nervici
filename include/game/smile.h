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
    const smileid_tu order;
    const smileid_tu count;
    const smilelvl_tu level;

    Point pos;

    bool visible;

    bool valid;

    Smile (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl);

public:
    virtual ~Smile ();

    virtual void step ();

    virtual void eat (plid_tu pid);

    virtual void erase ();

    virtual void update ();

    virtual SmileType get_type () const;

    virtual smilelvl_tu get_level () const;
};

class SmileFactory {
public:
    static Smile* create (smileid_tu sid, smileid_tu order, smileid_tu count,
            SmileType stype, smilelvl_tu lvl);
};

#endif	/* SMILE_H */
