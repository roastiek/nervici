/* 
 * File:   game_info.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 13:54
 * 
 * All information needed to start a game.
 */

#ifndef GAME_INFO_H
#define	GAME_INFO_H

#include <vector>

#include "smile_setting.h"
#include "basic_defs.h"
#include "mods/game_setting.h"
#include "fakes/pl_info.h"
#include "fakes/team_info.h"

struct GameInfo {
    /*
     * ID of mod, that shall be used.
     */
    int mod_id;
    
    /*
     * Base game setting as a speed, rounds...
     */
    GameSetting setting;
    
    /*
     * Counts of all smiles.
     */
    SmileSetting smiles;
    
    /*
     * List of teams for the game. I tcan be empty.
     */
    std::vector<const TeamInfo*> team_infos;
    
    /*
     * List of players for the game.
     */
    std::vector<const PlInfo*> pl_infos;
    
    /*
     * Maping between pl_infos and team_infos. Values are from 0 to 
     * teams_infos.size () inclusive, last value reprecents hidden "fake" 
     * default team, used for players without an explicit assigned team. Using
     * same indexing as pl_infos. 
     */
    std::vector<plid_tu> pl_teams;
};

#endif	/* GAME_INFO_H */

