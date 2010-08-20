#include "mods/nervici.h"
using namespace Nervici;

#include "mods/mod_interface.h"
//#include "game/player.h"

static const char * const exts[] = {
    NULL
};

static const ModRunnerInfo runner_info = {
    exts

};

static const ModInfo info = {
    "cervici",
    "bobo",
    "proste cervici",
    {16,
        1, 1, 1, 0, 1, 1, 1,
        12, 10, 0, 0, 1, 1000, 0,
        {
            {true, true, true},
            {true, true, true},
            {true, true, true},
            {true, true, true},
            {true, true, true},
            {true, true, true},
        },
        {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
        }}
};


#define WAIT_TIME 800

class Cervici : public ModInterface {
private:
    GameSetting set;

    void begin_start_procedure () {
        int sid;

        set_semafor (SEMAFOR_RED_1);
        game_wait (WAIT_TIME);

        for (int pi = 0; pi < set.playersCount; pi++) {
            IPlayer& p = get_player (pi);
            if (p.is_human ()) {
                sid = world_find_free_start ();
                if (sid < set.startsCount) {
                    p.give_start (sid);
                }
            }
        }
        set_semafor (SEMAFOR_RED_1 | SEMAFOR_RED_2);
        game_wait (WAIT_TIME);


        for (int pi = 0; pi < set.playersCount; pi++) {
            IPlayer& p = get_player (pi);
            if (!p.is_human ()) {
                sid = world_find_free_start ();
                if (sid < set.startsCount) {
                    p.give_start (sid);
                }
            }
        }
        set_semafor (SEMAFOR_RED_1 | SEMAFOR_RED_2 | SEMAFOR_RED_3);
        game_wait (WAIT_TIME);

        play_music (0);
        for (int pi = 0; pi < set.playersCount; pi++) {
            get_player (pi).start ();
        }
        set_semafor (SEMAFOR_GREEN);
    }

public:

    Cervici () {
    }

    const ModRunnerInfo* get_runner_info () {
        return &runner_info;
    }

    const ModInfo * get_info (const Glib::ustring & script) {
        return &info;
    }

    void on_game_start (const GameSetting & nset) {
        set = nset;

        begin_start_procedure ();
    }

    void on_death (IPlayer& player) {
        //player.revive ();
    }

    void after_step () {
        for (int pi = 0; pi < set.playersCount; pi++) {
            IPlayer& p = get_player (pi);
            if (p.is_live ()) {
                p.inc_score (1);
            }
        }

        if (live_pls_count () <= 0) {
            stop_music ();
            set_semafor (SEMAFOR_OFF);

            if (get_round () == set.rounds) {
                end_game ();
            } else {
                wait_for_space ();

                clear_playerground ();
                next_round ();
                begin_start_procedure ();
            }
        }
    }

    void on_pozi_smile (IPlayer& player, int lvl) {
        score_ti iron = player.get_ironize ();

        if (iron <= 0) {
            player.inc_score (set.bonus * lvl);
        } else {
            player.dec_score (set.bonus * lvl * iron);
            player.set_ironize (0);
        }
    }

    void on_nega_smile (IPlayer& player, int lvl) {
        score_ti iron = player.get_ironize ();

        if (iron <= 0) {
            player.dec_score (set.bonus * lvl);
        } else {
            player.inc_score (set.bonus * lvl + iron);
            player.set_ironize (0);
        }
    }

    void on_iron_smile (IPlayer& player, int lvl) {
        player.inc_ironize (lvl);
    }
};

ModInterface* get_face () {
    return new Cervici ();
}

