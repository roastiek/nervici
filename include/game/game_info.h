/* 
 * File:   game_info.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 13:54
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
    GameSetting setting;
    
    SmileSetting smiles;
    
    std::vector<const TeamInfo*> team_infos;
    
    std::vector<const PlInfo*> pl_infos;
    /*
     * maping between pl_infos and team_infos
     * values from 0..teams_infos.size () inclusive,
     * last value presents hidden "fake" default team,
     * so every player belong to some team 
     */
    std::vector<plid_tu> pl_teams;
};

#endif	/* GAME_INFO_H */

