

#include <math.h>

#include "game/smile.h"
#include "settings/pl_info.h"
#include "game/world.h"
#include "engine/render.h"

Smile::Smile (smileid_tu sid, smilelvl_tu lvl) :
id (sid),
level (lvl),
visible (false),
valid (true) {
}

Smile::~Smile () {
}

void Smile::step () {
}

void Smile::drop () {
}

void Smile::erase () {
    visible = false;
    valid = true;
}

void Smile::update () {
    if (!valid) {
        Render::update_smile (pos);
        valid = true;
    }
}

SmileType Smile::get_type () const {
    return ST_count;
}

smilelvl_tu Smile::get_level () const {
    return level;
}

class PoziSmile : public Smile {
private:
    int world_diagonal;
    int delay;

public:

    PoziSmile (smileid_tu sid, smilelvl_tu lvl) : Smile (sid, lvl) {
        world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        erase ();
    }

    void step () {
        if (!visible) {
            if (delay == 0) {
                for (int tries = 0; tries < 10; tries++) {
                    pos.x = random () % (World::get_width () - 20);
                    pos.y = random () % (World::get_height () - 20);
                    if (World::test_smile (id, pos)) {
                        World::write_soft_smile (id, pos);
                        Render::draw_smile (id, pos);
                        visible = true;
                        valid = false;
                        delay = world_diagonal / level + random () % 20;
                        break;
                    }
                }
            } else {
                delay--;
            }
        } else {
            if (delay == 0) {
                Render::clear_smile (pos);
                World::erase_smile (pos);
                visible = false;
                valid = false;
                delay = (random () % 20) * level;
            } else {
                delay--;
            }
        }
    }

    void drop () {
        Render::clear_smile (pos);
        World::erase_smile (pos);
        visible = false;
        valid = false;
        delay = (random () % 20) * level;
    }

    SmileType get_type () {
        return ST_pozi;
    }

    void erase () {
        Smile::erase ();
        delay = (random () % 20) * level;
    }

};

Smile* SmileFactory::create (smileid_tu sid, SmileType stype, smilelvl_tu lvl) {
    switch (stype) {
    case ST_pozi:
        return new PoziSmile (sid, lvl);
    default:
        return NULL;
    }
}