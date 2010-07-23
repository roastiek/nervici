

#include <math.h>

#include "game/smile.h"
#include "settings/pl_info.h"
#include "game/world.h"
#include "engine/render.h"
#include "system.h"
#include "engine/audio.h"
#include "game/players.h"

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
        step_length = world_diagonal * (4 - level) / 2;
        delay = world_diagonal * (4 - level) * order / 2 / count;
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

class NegaSmile : public Smile {
private:
    int world_diagonal;
    int delay;
    int step_length;

public:

    NegaSmile (smileid_tu sid, smileid_tu ord, smileid_tu co, smilelvl_tu lvl) :
    Smile (sid, ord, co, lvl) {
        world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 4;
        delay = world_diagonal * (4 - level) * order / 4 / count;
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
        System::mod_on_nega_smile (pid, level);
        Audio::play_effect (pid, ET_SmileMinus);
    }

    SmileType get_type () {
        return ST_nega;
    }

};

class FlegSmile : public Smile {
private:
    int world_diagonal;
    int delay;
    int step_length;

public:

    FlegSmile (smileid_tu sid, smileid_tu ord, smileid_tu co, smilelvl_tu lvl) :
    Smile (sid, ord, co, lvl) {
        world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level);
        delay = world_diagonal * (4 - level) * order / count;
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
                    World::write_hard_smile (id, pos);
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
        System::mod_on_fleg_smile (pid, level);
        Audio::play_effect (pid, ET_SmileMinus);
    }

    SmileType get_type () {
        return ST_fleg;
    }

};

class IronSmile : public Smile {
private:
    int world_diagonal;
    int delay;
    int step_length;

public:

    IronSmile (smileid_tu sid, smileid_tu ord, smileid_tu co, smilelvl_tu lvl) :
    Smile (sid, ord, co, lvl) {
        world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 2;
        delay = world_diagonal * (4 - level) * order / 2 / count;
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
        Players::ironize (pid);
        System::mod_on_iron_smile (pid, level);
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
    case ST_nega:
        return new NegaSmile (sid, order, count, lvl);
    case ST_fleg:
        return new FlegSmile (sid, order, count, lvl);
    case ST_iron:
        return new IronSmile (sid, order, count, lvl);
    default:
        return NULL;
    }
}