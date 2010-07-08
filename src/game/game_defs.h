/* 
 * File:   game_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 17:26
 */

#ifndef GAME_DEFS_H
#define	GAME_DEFS_H

#include <vector>

using namespace std;

#include "mods/mods.h"
#include "settings/plinfo_defs.h"

struct GameInfo {
    GameSetting *setting;
    vector<PlInfo> pl_infos;
};

#endif	/* GAME_DEFS_H */

