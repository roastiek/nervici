#ifndef __MODS_H__
#define __MODS_H__

#include "int_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ModRunnerInfo {
    const char * const *extensions;
} ModRunnerInfo;

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

typedef const ModRunnerInfo *(*ModGetModRunnerInfo) ();
typedef const ModInfo *(*ModGetModInfo) (const char* script);
typedef void (*LoadScript) (const char* script);
typedef void (*UnloadScript) ();
typedef void (*ModOnGameStart) (const GameSetting* set);
typedef void (*ModOnGameEnd) ();
typedef void (*ModOnTimer) ();
typedef void (*ModOnDeath) (plid_tu plid);
typedef void (*ModBeforeStep) ();
typedef void (*ModAfterStep) ();
typedef void (*ModOnPoziSmile) (int smid, int lvl);
typedef void (*ModOnNegaSmile) (int smid, int lvl);
typedef void (*ModOnFlegSmile) (int smid, int lvl);
typedef void (*ModOnIronSmile) (int smid, int lvl);
typedef void (*ModOnHamSmile) (int smid, int lvl);
typedef void (*ModOnKilled) (plid_tu plid, plid_tu murder);
typedef void (*ModOnKill) (plid_tu plid, plid_tu victim);
typedef void (*ModOnWall) (plid_tu plid);
typedef void (*ModOnSelfDeath) (plid_tu plid);
typedef void (*ModOnCleared) (plid_tu plid);
typedef void (*ModOnPlTimer) (plid_tu plid);

#ifdef	__cplusplus
}
#endif

#endif // __MODS_H__
