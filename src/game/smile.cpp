#include <math.h>
#include <cstdlib>

#include "basic_defs.h"
#include "main.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "mods/mod_interface.h"
#include "mods/mods.h"
#include "game/world.h"
#include "game/statistic.h"
#include "game/player.h"
#include "game/players.h"
#include "game/team.h"

#include "game/smile.h"

Smile::Smile (SmileType ntype, smileid_tu sid, smilelvl_tu lvl) :
    id (sid), level (lvl), type (ntype), visible (false) {
}

Smile::~Smile () {
}

void Smile::erase () {
    visible = false;
}

void Smile::eat (Player& pl) {
    pl.stat.smiles[type][level]++;
    pl.team.stat.smiles[type][level]++;
}

class SoftSmile: public Smile {
protected:
    int delay;
    int step_length;

public:

    SoftSmile (SmileType type, smileid_tu sid, smilelvl_tu lvl) :
        Smile (type, sid, lvl) {
    }

    void step () {
        if (delay <= 10 && visible) {
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
        }
        if (step_length - delay <= 1 && !visible) {
            if (found_new_pos ()) {
                World::write_soft_smile (id, pos);
                Render::draw_smile (id, pos);
                Render::update_smile (pos);
                visible = true;
            }
        }

        if (delay > 0) {
            delay--;
        } else {
            delay = step_length;
        }
    }

    virtual bool found_new_pos () {
        for (int tries = 0; tries < 10; tries++) {
            pos.x = random () % (World::get_width () - 20);
            pos.y = random () % (World::get_height () - 20);
            if (World::test_smile (id, pos)) {
                return true;
            }
        }
        return false;
    }

    void eat (Player& pl) {
        World::erase_smile (pos);
        Render::clear_smile (pos);
        Render::update_smile (pos);
        visible = false;
        Smile::eat (pl);
    }
};

class HardSmile: public Smile {
protected:
    int delay;
    int step_length;

public:

    HardSmile (SmileType type, smileid_tu sid, smilelvl_tu lvl) :
        Smile (type, sid, lvl) {
    }

    void step () {
        if (delay <= 10 && visible) {
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
        }
        if (step_length - delay <= 1 && !visible) {
            if (found_new_pos ()) {
                World::write_hard_smile (id, pos);
                Render::draw_smile (id, pos);
                Render::update_smile (pos);
                visible = true;
            }
        }

        if (delay > 0) {
            delay--;
        } else {
            delay = step_length;
        }
    }

    virtual bool found_new_pos () {
        for (int tries = 0; tries < 10; tries++) {
            pos.x = random () % (World::get_width () - 20);
            pos.y = random () % (World::get_height () - 20);
            if (World::test_smile (id, pos)) {
                return true;
            }
        }
        return false;
    }

};

class PoziSmile: public SoftSmile {
public:

    PoziSmile (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        SoftSmile (ST_pozi, sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width ()
                + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 2;
        delay = world_diagonal * (4 - level) * order / 2 / count;
    }

    void eat (Player& pl) {
        SoftSmile::eat (pl);
        mods.face ().on_pozi_smile (pl, level);
        Audio::play_effect (pl.id, ET_SmilePlus);
    }
};

class NegaSmileLvl1: public SoftSmile {
public:

    NegaSmileLvl1 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        SoftSmile (ST_nega, sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width ()
                + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 4;
        delay = world_diagonal * (4 - level) * order / 4 / count;
    }

    void eat (Player& pl) {
        SoftSmile::eat (pl);
        mods.face ().on_nega_smile (pl, level);
        Audio::play_effect (pl.id, ET_SmileMinus);
    }
};

class NegaSmileLvl2: public NegaSmileLvl1 {
public:

    NegaSmileLvl2 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        NegaSmileLvl1 (sid, order, count, lvl) {
    }

    bool found_new_pos () {
        for (int tries = 0; tries < 30; tries++) {
            plid_tu target = random () % players.count ();
            Player& pl = players[target];
            if (pl.is_live ()) {
                const FPoint& pl_pos = pl.get_position ();
                int angle = pl.get_angle ();

                pos.x = pl_pos.x + icos[angle] * (100 + random () % 20) - 10;
                pos.y = pl_pos.y + isin[angle] * (100 + random () % 20) - 10;

                if (World::test_smile (id, pos)) {
                    return true;
                }
            }
        }
        return false;
    }
};

class NegaSmileLvl3: public NegaSmileLvl1 {
public:

    NegaSmileLvl3 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        NegaSmileLvl1 (sid, order, count, lvl) {
    }

    bool found_new_pos () {
        for (int tries = 0; tries < 30; tries++) {
            plid_tu target = random () % players.count ();
            Player& pl = players[target];
            if (pl.is_live ()) {
                const FPoint& pl_pos = pl.get_position ();
                int angle = pl.get_angle ();

                pos.x = pl_pos.x + icos[angle] * (60 + random () % 20) - 10;
                pos.y = pl_pos.y + isin[angle] * (60 + random () % 20) - 10;

                if (World::test_smile (id, pos)) {
                    return true;
                }
            }
        }
        return false;
    }
};

class FlegSmileLvl1: public HardSmile {
public:

    FlegSmileLvl1 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        HardSmile (ST_fleg, sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width ()
                + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level);
        delay = world_diagonal * (4 - level) * order / count;
    }

    void eat (Player& pl) {
        HardSmile::eat (pl);
        mods.face ().on_fleg_smile (pl, level);
        Audio::play_effect (pl.id, ET_SmileMinus);
    }
};

class FlegSmileLvl2: public FlegSmileLvl1 {
public:

    FlegSmileLvl2 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        FlegSmileLvl1 (sid, order, count, lvl) {
    }

    bool found_new_pos () {
        for (int tries = 0; tries < 30; tries++) {
            plid_tu target = random () % players.count ();
            Player& pl = players[target];
            if (pl.is_live ()) {
                const FPoint& pl_pos = pl.get_position ();
                int angle = pl.get_angle ();

                pos.x = pl_pos.x + icos[angle] * (140 + random () % 20) - 10;
                pos.y = pl_pos.y + isin[angle] * (140 + random () % 20) - 10;

                if (World::test_smile (id, pos)) {
                    return true;
                }
            }
        }
        return false;
    }
};

class FlegSmileLvl3: public FlegSmileLvl1 {
public:

    FlegSmileLvl3 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        FlegSmileLvl1 (sid, order, count, lvl) {
    }

    bool found_new_pos () {
        for (int tries = 0; tries < 30; tries++) {
            plid_tu target = random () % players.count ();
            Player& pl = players[target];
            if (pl.is_live ()) {
                const FPoint& pl_pos = pl.get_position ();
                int angle = pl.get_angle ();

                pos.x = pl_pos.x + icos[angle] * (100 + random () % 20) - 10;
                pos.y = pl_pos.y + isin[angle] * (100 + random () % 20) - 10;

                if (World::test_smile (id, pos)) {
                    return true;
                }
            }
        }
        return false;
    }
};

class IronSmile: public SoftSmile {
public:

    IronSmile (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        SoftSmile (ST_iron, sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width ()
                + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 2;
        delay = world_diagonal * (4 - level) * order / 2 / count;
    }

    void eat (Player& pl) {
        SoftSmile::eat (pl);
        mods.face ().on_iron_smile (pl, level);
        Audio::play_effect (pl.id, ET_SmilePlus);
    }
};

class ChamSmileLvl1: public Smile {
private:
    int delay;
    int step_length;

protected:
    SmileType face_type;

public:

    ChamSmileLvl1 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        Smile (ST_cham, sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width ()
                + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 2;
        delay = world_diagonal * (4 - level) * order / 2 / count;
        face_type = ST_pozi;
    }

    void step () {
        if (delay <= 10 && visible) {
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
        }
        if (step_length - delay <= 1 && !visible) {
            for (int tries = 0; tries < 10; tries++) {
                pos.x = random () % (World::get_width () - 20);
                pos.y = random () % (World::get_height () - 20);
                if (World::test_smile (id, pos)) {
                    face_type = ST_pozi;
                    World::write_soft_smile (id, pos);
                    Render::draw_smile (id, pos, face_type);
                    Render::update_smile (pos);
                    visible = true;
                    break;
                }
            }
        }
        if (delay > 0 && visible) {
            if (try_swich_face ()) {
                World::erase_smile (pos);
                face_type = SmileType ((face_type + 1 + random () % 3) % 4);

                switch (face_type) {
                case ST_pozi:
                case ST_nega:
                case ST_iron:
                    World::write_soft_smile (id, pos);
                    break;
                default:
                    World::write_hard_smile (id, pos);
                    break;
                }
                Render::draw_smile (id, pos, face_type);
                Render::update_smile (pos);
            }
        }

        if (delay > 0) {
            delay--;
        } else {
            delay = step_length;
        }
    }

    virtual bool try_swich_face () {
        if ((step_length - delay) % 200 == 199) {
            face_type = SmileType ((face_type + 1 + random () % 3) % 4);
            return true;
        }
        return false;
    }
    ;

    void eat (Player& pl) {
        Smile::eat (pl);
        pl.stat.smiles[face_type][level]++;
        pl.team.stat.smiles[face_type][level]++;
        switch (face_type) {
        case ST_pozi:
        case ST_nega:
        case ST_iron:
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
            break;
        default:
            break;
        }
        switch (face_type) {
        case ST_pozi:
            mods.face ().on_pozi_smile (pl, level);
            Audio::play_effect (pl.id, ET_SmilePlus);
            break;
        case ST_nega:
            mods.face ().on_nega_smile (pl, level);
            Audio::play_effect (pl.id, ET_SmileMinus);
            break;
        case ST_fleg:
            mods.face ().on_fleg_smile (pl, level);
            Audio::play_effect (pl.id, ET_SmileMinus);
            break;
        case ST_iron:
            mods.face ().on_iron_smile (pl, level);
            Audio::play_effect (pl.id, ET_SmilePlus);
            break;
        default:
            break;
        }
    }
};

class ChamSmileLvl2: public ChamSmileLvl1 {
public:

    ChamSmileLvl2 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        ChamSmileLvl1 (sid, order, count, lvl) {
    }

    bool try_swich_face () {
        bool someone_near = false;
        for (size_t pi = 0; pi < players.count (); pi++) {
            Player& pl = players[pi];
            if (pl.is_live ()) {
                const FPoint& pl_pos = pl.get_position ();
                FPoint dist;
                dist.x = pl_pos.x - (pos.x + 10);
                dist.y = pl_pos.y - (pos.y + 10);
                someone_near
                        |= (sqrt (dist.x * dist.x + dist.y * dist.y) <= 80);
            }
        }

        if (someone_near) {
            if (face_type == ST_pozi || face_type == ST_iron) {
                face_type = (random () % 2 == 0) ? ST_nega : ST_fleg;
                return true;
            }
        } else {
            if (face_type == ST_nega || face_type == ST_fleg) {
                face_type = (random () % 2 == 0) ? ST_pozi : ST_iron;
                return true;
            }
        }
        return false;
    }
};

class ChamSmileLvl3: public ChamSmileLvl1 {
public:

    ChamSmileLvl3 (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        ChamSmileLvl1 (sid, order, count, lvl) {
    }

    bool try_swich_face () {
        bool someone_near = false;
        for (size_t pi = 0; pi < players.count (); pi++) {
            Player& pl = players[pi];
            if (pl.is_live ()) {
                const FPoint& pl_pos = pl.get_position ();
                FPoint dist;
                dist.x = pl_pos.x - (pos.x + 10);
                dist.y = pl_pos.y - (pos.y + 10);
                someone_near
                        |= (sqrt (dist.x * dist.x + dist.y * dist.y) <= 80);
            }
        }

        if (someone_near) {
            if (face_type == ST_pozi || face_type == ST_iron) {
                face_type = (random () % 2 == 0) ? ST_nega : ST_fleg;
                return true;
            }
        } else {
            if (face_type == ST_nega || face_type == ST_fleg) {
                face_type = (random () % 2 == 0) ? ST_pozi : ST_iron;
                return true;
            }
        }
        return false;
    }
};

class HamiSmile: public Smile {
private:
    int delay;
    int step_length;
    FPoint exact;
    int angle;
    int ani;

public:

    HamiSmile (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        Smile (ST_ham, sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width ()
                + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level);
        delay = world_diagonal * (4 - level) * order / count;
        ani = 0;
    }

    void step () {

        if (visible) {
            FPoint ftest = exact;
            ftest.x += icos[angle];
            ftest.y += isin[angle];
            Point test;
            test.x = ftest.x;
            test.y = ftest.y;

            if (test.x != pos.x || test.y != pos.y) {
                if (World::test_smile (id, test)) {
                    World::erase_smile (pos);
                    Render::clear_smile (pos);
                    pos = test;
                    exact = ftest;
                    World::write_hard_smile (id, pos);
                } else {
                    angle = random () % angles;
                }
            } else {
                exact = ftest;
            }
            Render::draw_smile (id, pos, ani / 20);
            Render::update_smile (pos);
            ani++;
            ani %= 40;
        }

        if (delay <= (random () % 20) && visible) {
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
        }
        if (delay <= 0 && !visible) {
            for (int tries = 0; tries < 10; tries++) {
                pos.x = random () % (World::get_width () - 20);
                pos.y = random () % (World::get_height () - 20);
                if (World::test_smile (id, pos)) {
                    World::write_hard_smile (id, pos);
                    Render::draw_smile (id, pos);
                    Render::update_smile (pos);

                    exact.x = pos.x;
                    exact.y = pos.y;
                    angle = random () % angles;

                    visible = true;
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

    void eat (Player& pl) {
        Smile::eat (pl);
        mods.face ().on_ham_smile (pl, level);
        Audio::play_effect (pl.id, ET_SmileMinus);
    }
};

class DestSmile: public Smile {
private:
    int delay;
    int step_length;
    FPoint exact;
    int angle;
    int ani;

public:

    DestSmile (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        Smile (ST_ham, sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width ()
                + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level);
        delay = world_diagonal * (4 - level) * order / count;
        ani = 0;
    }

    void step () {

        if (visible) {
            FPoint ftest = exact;
            ftest.x += icos[angle];
            ftest.y += isin[angle];
            Point test;
            test.x = ftest.x;
            test.y = ftest.y;

            if (test.x != pos.x || test.y != pos.y) {
                if (World::test_dest_smile (id, test)) {
                    World::erase_smile (pos);
                    Render::clear_smile (pos);
                    pos = test;
                    exact = ftest;
                    World::write_hard_smile (id, pos);
                } else {
                    angle = random () % angles;
                }
            } else {
                exact = ftest;
            }
            Render::draw_smile (id, pos, ani / 20);
            Render::update_smile (pos);
            ani++;
            ani %= 40;
        }

        if (delay <= (random () % 20) && visible) {
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
        }
        if (delay <= 0 && !visible) {
            for (int tries = 0; tries < 10; tries++) {
                pos.x = random () % (World::get_width () - 20);
                pos.y = random () % (World::get_height () - 20);
                if (World::test_smile (id, pos)) {
                    World::write_hard_smile (id, pos);
                    Render::draw_smile (id, pos);
                    Render::update_smile (pos);

                    exact.x = pos.x;
                    exact.y = pos.y;
                    angle = random () % angles;

                    visible = true;
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

    void eat (Player& pl) {
        Smile::eat (pl);
        mods.face ().on_ham_smile (pl, level);
        Audio::play_effect (pl.id, ET_SmileMinus);
    }
};

class TermSmile: public Smile {
private:
    int delay;
    int step_length;
    FPoint exact;
    int angle;
    int ani;

public:

    TermSmile (smileid_tu sid, smileid_tu order, smileid_tu count,
            smilelvl_tu lvl) :
        Smile (ST_ham, sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width ()
                + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level);
        delay = world_diagonal * (4 - level) * order / count;
        ani = 0;
    }

    void step () {

        if (visible) {
            if (random () % 20 == 0) {
                angle += angles + random () % 41 - 20;
                angle %= angles;
            }

            FPoint ftest = exact;
            ftest.x += icos[angle];
            ftest.y += isin[angle];
            Point test;
            test.x = ftest.x;
            test.y = ftest.y;

            if (test.x != pos.x || test.y != pos.y) {
                if (World::test_dest_smile (id, test)) {
                    World::erase_smile (pos);
                    Render::clear_smile (pos);
                    pos = test;
                    exact = ftest;
                    World::write_hard_smile (id, pos);
                } else {
                    angle = random () % angles;
                }
            } else {
                exact = ftest;
            }
            Render::draw_smile (id, pos, ani / 20);
            Render::update_smile (pos);
            ani++;
            ani %= 40;
        }

        if (delay <= (random () % 20) && visible) {
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
        }
        if (delay <= 0 && !visible) {
            for (int tries = 0; tries < 10; tries++) {
                pos.x = random () % (World::get_width () - 20);
                pos.y = random () % (World::get_height () - 20);
                if (World::test_smile (id, pos)) {
                    World::write_hard_smile (id, pos);
                    Render::draw_smile (id, pos);
                    Render::update_smile (pos);

                    exact.x = pos.x;
                    exact.y = pos.y;
                    angle = random () % angles;

                    visible = true;
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

    void eat (Player& pl) {
        Smile::eat (pl);
        mods.face ().on_ham_smile (pl, level);
        Audio::play_effect (pl.id, ET_SmileMinus);
    }

    SmileType get_type () const {
        return ST_ham;
    }
};

Smile* SmileFactory::create (smileid_tu sid, smileid_tu order,
        smileid_tu count, SmileType stype, smilelvl_tu lvl) {
    switch (stype) {
    case ST_pozi:
        return new PoziSmile (sid, order, count, lvl);
    case ST_nega:
        switch (lvl) {
        case 2:
            return new NegaSmileLvl2 (sid, order, count, lvl);
        case 3:
            return new NegaSmileLvl3 (sid, order, count, lvl);
        default:
            return new NegaSmileLvl1 (sid, order, count, lvl);
        }
    case ST_fleg:
        switch (lvl) {
        case 2:
            return new FlegSmileLvl2 (sid, order, count, lvl);
        case 3:
            return new FlegSmileLvl3 (sid, order, count, lvl);
        default:
            return new FlegSmileLvl1 (sid, order, count, lvl);
        }
    case ST_iron:
        return new IronSmile (sid, order, count, lvl);
    case ST_cham:
        switch (lvl) {
        case 2:
            return new ChamSmileLvl2 (sid, order, count, lvl);
        case 3:
            return new ChamSmileLvl3 (sid, order, count, lvl);
        default:
            return new ChamSmileLvl1 (sid, order, count, lvl);
        }
    case ST_ham:
        switch (lvl) {
        case 2:
            return new DestSmile (sid, order, count, lvl);
        case 3:
            return new TermSmile (sid, order, count, lvl);
        default:
            return new HamiSmile (sid, order, count, lvl);
        }
    default:
        return NULL;
    }
}
