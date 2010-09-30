#include <cstddef>
#include <glibmm/ustring.h>

#include "basic_defs.h"
#include "mods/nervici.h"
#include "mods/mod_runner_info.h"
#include "mods/mod_info.h"
#include "mods/game_setting.h"

using namespace Nervici;
using namespace Glib;

#include "mods/mod_interface.h"

static const char * const exts[] = {
    NULL};

static const ModRunnerInfo runner_info = {
    exts

};

static ModInfo info = {
    "cervici",
    N_("worms"),
    "bobo",
    N_("proste cervici"),
    {
        16,
        true,
        true,
        true,
        false,
        false,
        false,
        true,
        {
            {
                false,
                false,
                false},
            {
                false,
                false,
                false},
            {
                false,
                false,
                false},
            {
                false,
                false,
                false},
            {
                false,
                false,
                false},
            {
                false,
                false,
                false}, },
        {
            12,
            10,
            0,
            0,
            1,
            0,
            0},
        {
            {
                {
                    0,
                    0,
                    0},
                {
                    0,
                    0,
                    0},
                {
                    0,
                    0,
                    0},
                {
                    0,
                    0,
                    0},
                {
                    0,
                    0,
                    0},
                {
                    0,
                    0,
                    0}, }}}};

#define WAIT_TIME 800

class Cervici: public ModInterface {
private:
    GameSetting set;

    void begin_start_procedure () {
        int sid;

        if (set.game_time != 0) {
            set_timer (set.game_time * -1000);
        }

        set_status (_("prepare"));
        set_semafor (SEMAFOR_RED_1);
        game_wait (WAIT_TIME);

        for (int pi = 0; pi < get_players_count (); pi++) {
            IPlayer& p = get_player (pi);
            if (p.is_human ()) {
                sid = world_find_free_start ();
                if (sid < get_starts_count ()) {
                    p.give_start (sid);
                }
            }
        }
        set_semafor (SEMAFOR_RED_1 | SEMAFOR_RED_2);
        game_wait (WAIT_TIME);

        for (int pi = 0; pi < get_players_count (); pi++) {
            IPlayer& p = get_player (pi);
            if (!p.is_human ()) {
                sid = world_find_free_start ();
                if (sid < get_starts_count ()) {
                    p.give_start (sid);
                }
            }
        }
        set_semafor (SEMAFOR_RED_1 | SEMAFOR_RED_2 | SEMAFOR_RED_3);
        game_wait (WAIT_TIME);
        set_status (_("run"));

        play_music (0);
        for (int pi = 0; pi < get_players_count (); pi++) {
            get_player (pi).start ();
        }
        set_semafor (SEMAFOR_GREEN);
    }

    void end_round () {
        if (set.rounds > 0 && get_round () == set.rounds) {
            end_game ();
        } else {
            wait_for_space ();
            clear_playerground ();
            next_round ();
            begin_start_procedure ();
        }
    }

public:

    Cervici () {
    }

    const ModRunnerInfo* get_runner_info () {
        return &runner_info;
    }

    const ModInfo * get_info (const Glib::ustring & script) {
        static bool translated = false;
        if (!translated) {
            info.name = gettext (info.name);
            info.rules = gettext (info.rules);
            translated = true;
        }

        return &info;
    }

    void on_game_start (const GameSetting & nset, const SmileSetting& smiles) {
        set = nset;

        begin_start_procedure ();
    }

    void after_step () {
        int last_living = -1;
        int max_score = get_player (0).get_score ();

        for (int pi = 0; pi < get_players_count (); pi++) {
            IPlayer& p = get_player (pi);
            if (p.is_live ()) {
                if (!p.is_jumping ()) {
                    p.inc_score (1);
                }
                last_living = pi;
            }
            if (p.get_score () > max_score)
                max_score = p.get_score ();
        }

        if (live_pls_count () <= 1) {
            stop_music ();
            set_semafor (SEMAFOR_OFF);

            if (last_living != -1) {
                set_status (_("player %s has won this round"),
                    get_player (last_living).get_name ().c_str ());
            } else {
                set_status (_("no winner this round"));
            }
            end_round ();
        }
        if (set.max_score > 0) {
            if (max_score >= set.max_score) {
                set_status(_("final score has been reached"));
                end_game ();
            }
        }
    }

    void on_timer () {
        set_status (_("time has run out"));
        end_round ();
    }

    /*void on_pozi_smile (IPlayer& player, int lvl) {
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
     }*/
};

ModInterface* get_cervici_face () {
    return new Cervici ();
}

