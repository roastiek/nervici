/* 
 * File:   mod_specification.h
 * Author: bobo
 *
 * Created on 27. červenec 2010, 12:02
 */

#ifndef MOD_SPECIFICATION_H
#define	MOD_SPECIFICATION_H

#include "game/smile_type.h"

struct ModSpecification {
    plid_tu max_players;
    int speed_enabled;
    int rounds_enabled;
    int max_score_enabled;
    int max_length_enabled;
    int step_enabled;
    int bonus_enabled;
    int timer_enabled;
    timer_ti default_speed;
    round_tu default_rounds;
    score_ti default_max_score;
    plsize_tu default_max_length;
    score_ti default_step;
    score_ti default_bonus;
    timer_ti default_timer;
    bool smiles_enabled[ST_count][3];
    uint8_t defualt_smiles_counts[ST_count][3];
};

#endif	/* MOD_SPECIFICATION_H */

