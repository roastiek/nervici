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
    round_tu rounds;
    score_ti bonus;
    timer_ti game_time;
    timer_ti speed;
    plsize_tu max_length;
    score_ti max_score;
    score_ti step;
};

#endif	/* GAME_SETTING_H */

