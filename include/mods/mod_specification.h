/* 
 * File:   mod_specification.h
 * Author: bobo
 *
 * Created on 27. červenec 2010, 12:02
 */

#ifndef MOD_SPECIFICATION_H
#define	MOD_SPECIFICATION_H

#include "game/smile_type.h"
#include "mods/game_setting.h"
#include "game/smile_setting.h"

struct ModSpecification {
    plid_tu max_players;
    bool speed_enabled;
    bool rounds_enabled;
    bool max_score_enabled;
    bool max_length_enabled;
    bool timer_enabled;
    bool step_enabled;
    bool bonus_enabled;
    bool smiles_enabled[ST_count][3];
    GameSetting defaults;
    SmileSetting default_smiles;
};

#endif	/* MOD_SPECIFICATION_H */

