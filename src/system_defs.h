/* 
 * File:   system_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 18:15
 */

#ifndef SYSTEM_DEFS_H
#define	SYSTEM_DEFS_H

#include <string>

using namespace std;

struct ModEvents {
    ModOnGameStart on_game_start;
    ModOnGameEnd on_game_end;
    ModOnTimer on_timer;
    ModOnDeath on_death;
    ModBeforeStep before_step;
    ModAfterStep after_step;
    ModOnPoziSmile on_pozi_smile;
    ModOnNegaSmile on_nega_smile;
    ModOnFlegSmile on_fleg_smile;
    ModOnIronSmile on_iron_smile;
    ModOnHamSmile on_ham_smile;
    ModOnKilled on_killed;
    ModOnKill on_kill;
    ModOnWall on_wall;
    ModOnSelfDeath on_self_death;
    ModOnCleared on_cleared;
    ModOnPlTimer on_pl_timer;
};

struct ModEntry {
    string filename;
    const ModInfo *info;
};


#endif	/* SYSTEM_DEFS_H */
