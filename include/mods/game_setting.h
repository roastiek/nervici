/* 
 * File:   game_setting.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 20:38
 */

#ifndef GAME_SETTING_H
#define	GAME_SETTING_H

#ifdef __cplusplus
extern "C" {
#endif

struct GameSetting {
    int playersCount;
    int teams_count;
    int startsCount;
    int rounds;
    int bonus;
    int gameTime;
    int speed;
    int maxLength;
    int maxScore;
    int step;
};
   

#ifdef	__cplusplus
}
#endif


#endif	/* GAME_SETTING_H */

