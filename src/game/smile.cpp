#include <math.h>

#include "game/smile.h"
#include "settings/pl_info.h"
#include "game/world.h"
#include "engine/render.h"
#include "system.h"
#include "engine/audio.h"
#include "game/players.h"
#include "main.h"
#include "game/player.h"

Smile::Smile (smileid_tu sid, smilelvl_tu lvl) :
id (sid),
level (lvl),
visible (false) {
}

Smile::~Smile () {
}

void Smile::erase () {
    visible = false;
}

smilelvl_tu Smile::get_level () const {
    return level;
}

class SoftSmile : public Smile {
protected:
    int delay;
    int step_length;

public:

    SoftSmile (smileid_tu sid, smilelvl_tu lvl) :
    Smile (sid, lvl) {
    };

    void step () {
        if (delay <= 10 && visible) {
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
        }
        if (step_length - delay <= 10 && !visible) {
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

    void eat (plid_tu plid) {
        World::erase_smile (pos);
        Render::clear_smile (pos);
        Render::update_smile (pos);
        visible = false;
    }
};

class HardSmile : public Smile {
protected:
    int delay;
    int step_length;

public:

    HardSmile (smileid_tu sid, smilelvl_tu lvl) :
    Smile (sid, lvl) {
    };

    void step () {
        if (delay <= 10 && visible) {
            World::erase_smile (pos);
            Render::clear_smile (pos);
            Render::update_smile (pos);
            visible = false;
        }
        if (step_length - delay <= 10 && !visible) {
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

class PoziSmile : public SoftSmile {
public:

    PoziSmile (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    SoftSmile (sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 2;
        delay = world_diagonal * (4 - level) * order / 2 / count;
    }

    void eat (plid_tu plid) {
        SoftSmile::eat (plid);

        Players::stat (plid).smiles[ST_pozi][level]++;
        Players::team_stat (plid).smiles[ST_pozi][level]++;
        System::mod->on_pozi_smile (Players::get_player (plid), level);
        Audio::play_effect (plid, ET_SmilePlus);
    }

    SmileType get_type () const {
        return ST_pozi;
    }

};

class NegaSmileLvl1 : public SoftSmile {
public:

    NegaSmileLvl1 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    SoftSmile (sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 4;
        delay = world_diagonal * (4 - level) * order / 4 / count;
    }

    void eat (plid_tu plid) {
        SoftSmile::eat (plid);

        Players::stat (plid).smiles[ST_nega][level]++;
        Players::team_stat (plid).smiles[ST_nega][level]++;
        System::mod->on_nega_smile (Players::get_player (plid), level);
        Audio::play_effect (plid, ET_SmileMinus);
    }

    SmileType get_type () const {
        return ST_nega;
    }
};

class NegaSmileLvl2 : public NegaSmileLvl1 {
public:

    NegaSmileLvl2 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    NegaSmileLvl1 (sid, order, count, lvl) {
    }

    bool found_new_pos () {
        for (int tries = 0; tries < 30; tries++) {
            plid_tu target = random () % Players::get_count ();
            if (Players::is_pl_live (target)) {
                const FPoint& pl_pos = Players::get_pl_position (target);
                int angle = Players::get_pl_angle (target);

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

class NegaSmileLvl3 : public NegaSmileLvl1 {
public:

    NegaSmileLvl3 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    NegaSmileLvl1 (sid, order, count, lvl) {
    }

    bool found_new_pos () {
        for (int tries = 0; tries < 30; tries++) {
            plid_tu target = random () % Players::get_count ();
            if (Players::is_pl_live (target)) {
                const FPoint& pl_pos = Players::get_pl_position (target);
                int angle = Players::get_pl_angle (target);

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

class FlegSmileLvl1 : public HardSmile {
public:

    FlegSmileLvl1 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    HardSmile (sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level);
        delay = world_diagonal * (4 - level) * order / count;
    }

    void eat (plid_tu plid) {
        Players::stat (plid).smiles[ST_fleg][level]++;
        Players::team_stat (plid).smiles[ST_fleg][level]++;
        System::mod->on_fleg_smile (Players::get_player (plid), level);
        Audio::play_effect (plid, ET_SmileMinus);
    }

    SmileType get_type () const {
        return ST_fleg;
    }

};

class FlegSmileLvl2 : public FlegSmileLvl1 {
public:

    FlegSmileLvl2 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    FlegSmileLvl1 (sid, order, count, lvl) {
    }

    bool found_new_pos () {
        for (int tries = 0; tries < 30; tries++) {
            plid_tu target = random () % Players::get_count ();
            if (Players::is_pl_live (target)) {
                const FPoint& pl_pos = Players::get_pl_position (target);
                int angle = Players::get_pl_angle (target);

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

class FlegSmileLvl3 : public FlegSmileLvl1 {
public:

    FlegSmileLvl3 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    FlegSmileLvl1 (sid, order, count, lvl) {
    }

    bool found_new_pos () {
        for (int tries = 0; tries < 30; tries++) {
            plid_tu target = random () % Players::get_count ();
            if (Players::is_pl_live (target)) {
                const FPoint& pl_pos = Players::get_pl_position (target);
                int angle = Players::get_pl_angle (target);

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

class IronSmile : public SoftSmile {
public:

    IronSmile (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    SoftSmile (sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level) / 2;
        delay = world_diagonal * (4 - level) * order / 2 / count;
    }

    void eat (plid_tu plid) {
        SoftSmile::eat (plid);

        Players::stat (plid).smiles[ST_iron][level]++;
        Players::team_stat (plid).smiles[ST_iron][level]++;
        System::mod->on_iron_smile (Players::get_player (plid), level);
        Audio::play_effect (plid, ET_SmilePlus);
    }

    SmileType get_type () const {
        return ST_pozi;
    }

};

class ChamSmileLvl1 : public Smile {
private:
    int delay;
    int step_length;

protected:
    SmileType face_type;

public:

    ChamSmileLvl1 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    Smile (sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
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
        if (step_length - delay <= 10 && !visible) {
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
    };

    void eat (plid_tu plid) {
        Players::stat (plid).smiles[face_type][level]++;
        Players::team_stat (plid).smiles[face_type][level]++;
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
            System::mod->on_pozi_smile (Players::get_player (plid), level);
            Audio::play_effect (plid, ET_SmilePlus);
            break;
        case ST_nega:
            System::mod->on_nega_smile (Players::get_player (plid), level);
            Audio::play_effect (plid, ET_SmileMinus);
            break;
        case ST_fleg:
            System::mod->on_fleg_smile (Players::get_player (plid), level);
            Audio::play_effect (plid, ET_SmileMinus);
            break;
        case ST_iron:
            System::mod->on_iron_smile (Players::get_player (plid), level);
            Audio::play_effect (plid, ET_SmilePlus);
            break;
        default:
            break;
        }
    }

    SmileType get_type () const {
        return face_type;
    }

};

class ChamSmileLvl2 : public ChamSmileLvl1 {
public:

    ChamSmileLvl2 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    ChamSmileLvl1 (sid, order, count, lvl) {
    }

    bool try_swich_face () {
        bool someone_near = false;
        for (size_t pi = 0; pi < Players::get_count (); pi++) {
            if (Players::is_pl_live (pi)) {
                const FPoint& pl_pos = Players::get_pl_position (pi);
                FPoint dist;
                dist.x = pl_pos.x - (pos.x + 10);
                dist.y = pl_pos.y - (pos.y + 10);
                someone_near |= (sqrt (dist.x * dist.x + dist.y * dist.y) <= 80);
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

class ChamSmileLvl3 : public ChamSmileLvl1 {
public:

    ChamSmileLvl3 (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    ChamSmileLvl1 (sid, order, count, lvl) {
    }

    bool try_swich_face () {
        bool someone_near = false;
        for (size_t pi = 0; pi < Players::get_count (); pi++) {
            if (Players::is_pl_live (pi)) {
                const FPoint& pl_pos = Players::get_pl_position (pi);
                FPoint dist;
                dist.x = pl_pos.x - (pos.x + 10);
                dist.y = pl_pos.y - (pos.y + 10);
                someone_near |= (sqrt (dist.x * dist.x + dist.y * dist.y) <= 80);
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

class HamiSmile : public Smile {
private:
    int delay;
    int step_length;
    FPoint exact;
    int angle;
    int ani;

public:

    HamiSmile (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    Smile (sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level);
        delay = world_diagonal * (4 - level) * order / count;
        ani = 0;
    }

    void step () {

        if (visible) {
            FPoint ftest = exact;
            ftest.x += icos [angle];
            ftest.y += isin [angle];
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

    void eat (plid_tu plid) {
        Players::stat (plid).smiles[ST_ham][level]++;
        Players::team_stat (plid).smiles[ST_ham][level]++;
        System::mod->on_iron_smile (Players::get_player (plid), level);
        Audio::play_effect (plid, ET_SmilePlus);
    }

    SmileType get_type () const {
        return ST_ham;
    }
};

class DestSmile : public Smile {
private:
    int delay;
    int step_length;
    FPoint exact;
    int angle;
    int ani;

public:

    DestSmile (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    Smile (sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
        step_length = world_diagonal * (4 - level);
        delay = world_diagonal * (4 - level) * order / count;
        ani = 0;
    }

    void step () {

        if (visible) {
            FPoint ftest = exact;
            ftest.x += icos [angle];
            ftest.y += isin [angle];
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

    void eat (plid_tu plid) {
        Players::stat (plid).smiles[ST_ham][level]++;
        Players::team_stat (plid).smiles[ST_ham][level]++;
        System::mod->on_iron_smile (Players::get_player (plid), level);
        Audio::play_effect (plid, ET_SmilePlus);
    }

    SmileType get_type () const {
        return ST_ham;
    }
};

class TermSmile : public Smile {
private:
    int delay;
    int step_length;
    FPoint exact;
    int angle;
    int ani;

public:

    TermSmile (smileid_tu sid, smileid_tu order, smileid_tu count, smilelvl_tu lvl) :
    Smile (sid, lvl) {
        int world_diagonal = sqrt (World::get_width () * World::get_width () + World::get_height () * World::get_height ());
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
            ftest.x += icos [angle];
            ftest.y += isin [angle];
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

    void eat (plid_tu plid) {
        Players::stat (plid).smiles[ST_ham][level]++;
        Players::team_stat (plid).smiles[ST_ham][level]++;
        System::mod->on_iron_smile (Players::get_player (plid), level);
        Audio::play_effect (plid, ET_SmilePlus);
    }

    SmileType get_type () const {
        return ST_ham;
    }
};

Smile* SmileFactory::create (smileid_tu sid, smileid_tu order, smileid_tu count,
        SmileType stype, smilelvl_tu lvl) {
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