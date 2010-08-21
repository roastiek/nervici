/* 
 * File:   game_setting.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 20:38
 */

#ifndef GAME_SETTING_H
#define	GAME_SETTING_H

struct GameSetting {
//    int players_count;
    int teams_count;
    int starts_count;
    int rounds;
    int bonus;
    int gameTime;
    int speed;
    int maxLength;
    int maxScore;
    int step;
};

#endif	/* GAME_SETTING_H */

