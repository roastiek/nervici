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
#include "game/smile_type.h"
#include "fakes/player.h"

class Smile {
protected:
    const smileid_tu id;

    const smilelvl_tu level;

    const SmileType type;

    Point pos;

    bool visible;

    Smile (SmileType type, smileid_tu sid, smilelvl_tu lvl);

public:
    virtual ~Smile ();

    virtual void step () = 0;

    virtual void eat (Player& pl);

    virtual void erase ();
};

class SmileFactory {
public:
    static Smile* create (smileid_tu sid, smileid_tu order, smileid_tu count,
            SmileType stype, smilelvl_tu lvl);
};

#endif	/* SMILE_H */

