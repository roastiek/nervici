#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nervici.h"
#include "mods.h"

static ModInfo info = {
    "cervici",
    "bobo",
    "proste cervici"
};

static const GameSetting *set;
//static int delta = -1;

const ModInfo *get_mod_info () {
    return &info;
}

void on_game_start (const GameSetting *nset) {
    int p, s;

    set = nset;
    
    for (p = 0; p < set->playersCount; p++) {
        printf ("game start %d\n", p);
        s = world_find_free_start ();
        if (s != -1) {
            give_pl_start (p, s);
            set_pl_timer (p, -1000);
        }
    }
    printf ("game start\n");
    
    set_timer (-3000);
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

void on_kill (int plid,int victim) {
}

void on_wall (int plid) {
}

void on_self_death (int plid) {
}

void on_cleared (int plid) {
}

void on_pl_timer (int plid) {
    start_pl (plid);
    play_music (0);
    set_semafor (1);
    next_round ();
}
