/* 
 * File:   game_info.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 13:54
 */

#ifndef GAME_INFO_H
#define	GAME_INFO_H

#include "mods/game_setting.h"
#include "basic_defs.h"

struct GameInfo {
    GameSetting setting;
    bool team_active[TEAMS_COUNT];
    int pl_ids[16];
    int pls_team[16];
};



#endif	/* GAME_INFO_H */

