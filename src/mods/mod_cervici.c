#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nervici.h"
#include "mods.h"

static const char * const exts[] = {
    "", NULL
};

static ModRunnerInfo runner_info = {
    NULL
};

static ModInfo info = {
    "cervici",
    "bobo",
    "proste cervici",
};

static const GameSetting *set;

const ModRunnerInfo *get_mod_runner_info () {
    return &runner_info;
}

const ModInfo *get_mod_info (const char* script) {
    return &info;
}

#define WAIT_TIME 800

static void begin_start_procedure () {
    int sid;

    set_semafor (SEMAFOR_R1);
    game_wait (WAIT_TIME);

    for (int pi = 0; pi < set->playersCount; pi++) {
        if (is_pl_human (pi)) {
            sid = world_find_free_start ();
            if (sid < set->startsCount) {
                give_pl_start (pi, sid);
            }
        }
    }
    set_semafor (SEMAFOR_R1 | SEMAFOR_R2);
    game_wait (WAIT_TIME);


    for (int pi = 0; pi < set->playersCount; pi++) {
        if (!is_pl_human (pi)) {
            sid = world_find_free_start ();
            if (sid < set->startsCount) {
                give_pl_start (pi, sid);
            }
        }
    }
    set_semafor (SEMAFOR_R1 | SEMAFOR_R2 | SEMAFOR_R3);
    game_wait (WAIT_TIME);

    play_music (0);
    for (int pi = 0; pi < set->playersCount; pi++) {
        start_pl (pi);
    }
    set_semafor (SEMAFOR_G1);
}

void load_script (const char* script) {
}

void unload_script () {
}

void on_game_start (const GameSetting *nset) {
    set = nset;

    begin_start_procedure ();
}

void on_game_end () {
}

void on_timer () {
}

void on_death (int plid) {
}

void before_step () {
}

void after_step () {
    int p;

    for (p = 0; p < set->playersCount; p++) {
        if (is_pl_live (p)) {
            inc_pl_score (p, 1);
        }
    }

    if (live_pls_count () <= 0) {
        stop_music ();
        set_semafor (SEMAFOR_OF);

        if (get_round () == set->rounds) {
            end_game ();
        } else {
            wait_for_space ();

            clear_playerground ();
            next_round ();
            begin_start_procedure ();
        }
    }
}

void on_pozi_smile (int smid, int lvl) {
}

void on_nega_smile (int smid, int lvl) {
}

void on_fleg_smile (int smid, int lvl) {
}

void on_iron_smile (int smid, int lvl) {
}

void on_ham_smile (int smid, int lvl) {
}

void on_killed (int plid, int murder) {
}

void on_kill (int plid, int victim) {
}

void on_wall (int plid) {
}

void on_self_death (int plid) {
}

void on_cleared (int plid) {
}

void on_pl_timer (int plid) {
}
