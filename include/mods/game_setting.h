/* 
 * File:   game_setting.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 20:38
 */

#ifndef GAME_SETTING_H
#define	GAME_SETTING_H

#include "int_type.h"

struct GameSetting {
    timer_ti speed;
    round_tu rounds;
    score_ti max_score;
    plsize_tu max_length;
    timer_ti game_time;
    score_ti step;
    score_ti bonus;
};

#endif	/* GAME_SETTING_H */

