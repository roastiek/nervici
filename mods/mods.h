#ifndef __MODS_H__
#define __MODS_H__

typedef struct ModInfo {
    const char *name;
    const char *autor;
    const char *rules;
} ModInfo;

typedef struct GameSetting {
    int playersCount;
    int startsCount;
    int rounds;
    int bonus;
    int gameTime;
    int speed;
    int maxLength;
    int maxScore;
} GameSetting;

typedef ModInfo *(*pro_getModInfo) ();
typedef void (*pro_onGameStart) (const GameSetting *set);
typedef void (*pro_onGameEnd) ();
typedef void (*pro_onTimer) ();
typedef void (*pro_onDeath) (int plid);
typedef void (*pro_beforeStep) ();
typedef void (*pro_afterStep) ();
typedef void (*pro_onPoziSmile) (int smid, int lvl);
typedef void (*pro_onNegaSmile) (int smid, int lvl); 
typedef void (*pro_onFlegSmile) (int smid, int lvl); 
typedef void (*pro_onIronSmile) (int smid, int lvl); 
typedef void (*pro_onHamSmile) (int smid, int lvl); 
typedef void (*pro_onKilled) (int plid, int murder);
typedef void (*pro_onKill) (int plid,int victim);
typedef void (*pro_onWall) (int plid);
typedef void (*pro_onSelfDeath) (int plid);
typedef void (*pro_onCleared) (int plid);
typedef void (*pro_onPlTimer) (int plid);


#endif // __MODS_H__
