

#include <math.h>

#include "game/smile.h"
#include "settings/pl_info.h"
#include "game/world.h"
#include "engine/render.h"
#include "system.h"
#include "engine/audio.h"

Smile::Smile (smileid_tu sid, smileid_tu ord, smileid_tu co, smilelvl_tu lvl) :
id (sid),
order (ord),
count (co),
level (lvl),
visible (false),
valid (true) {
}

Smile::~Smile () {
}

void Smile::step () {
}

void Smile::eat (plid_tu pid) {
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
    int step_length;

public:

    PoziSmile (smileid_tu sid, smileid_tu ord, smileid_tu co, smilelvl_tu lvl) :
    Smile (sid, ord, co, lvl) {
        world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 3;
        delay = world_diagonal * (4 - level) * order / 3 / count;
    }

    void step () {
        if (delay <= (random () % 20) && visible) {
            Render::clear_smile (pos);
            World::erase_smile (pos);
            visible = false;
            valid = false;
        }
        if (delay <= 0 && !visible) {
            for (int tries = 0; tries < 10; tries++) {
                pos.x = random () % (World::get_width () - 20);
                pos.y = random () % (World::get_height () - 20);
                if (World::test_smile (id, pos)) {
                    World::write_soft_smile (id, pos);
                    Render::draw_smile (id, pos);
                    visible = true;
                    valid = false;
                    break;
                }
            }
        }

        if (delay > 0) {
            delay--;
        } else {
            delay = step_length;
        }
    }

    void eat (plid_tu pid) {
        Render::clear_smile (pos);
        World::erase_smile (pos);
        visible = false;
        valid = false;
        System::mod_on_pozi_smile (pid, level);
        Audio::play_effect (pid, ET_SmilePlus);
    }

    SmileType get_type () {
        return ST_pozi;
    }

};

Smile* SmileFactory::create (smileid_tu sid, smileid_tu order, smileid_tu count,
        SmileType stype, smilelvl_tu lvl) {
    switch (stype) {
    case ST_pozi:
        return new PoziSmile (sid, order, count, lvl);
    default:
        return NULL;
    }
}