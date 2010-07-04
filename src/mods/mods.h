#ifndef __MODS_H__
#define __MODS_H__

typedef struct _ModInfo {
    const char *name;
    const char *autor;
    const char *rules;
} ModInfo;

typedef struct _GameSetting {
    int playersCount;
    int startsCount;
    int rounds;
    int bonus;
    int gameTime;
    int speed;
    int maxLength;
    int maxScore;
} GameSetting;

typedef const ModInfo *(*ModGetModInfo) ();
typedef void (*ModOnGameStart) (const GameSetting *set);
typedef void (*ModOnGameEnd) ();
typedef void (*ModOnTimer) ();
typedef void (*ModOnDeath) (int plid);
typedef void (*ModBeforeStep) ();
typedef void (*ModAfterStep) ();
typedef void (*ModOnPoziSmile) (int smid, int lvl);
typedef void (*ModOnNegaSmile) (int smid, int lvl);
typedef void (*ModOnFlegSmile) (int smid, int lvl);
typedef void (*ModOnIronSmile) (int smid, int lvl);
typedef void (*ModOnHamSmile) (int smid, int lvl);
typedef void (*ModOnKilled) (int plid, int murder);
typedef void (*ModOnKill) (int plid,int victim);
typedef void (*ModOnWall) (int plid);
typedef void (*ModOnSelfDeath) (int plid);
typedef void (*ModOnCleared) (int plid);
typedef void (*ModOnPlTimer) (int plid);


#endif // __MODS_H__
