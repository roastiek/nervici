#ifndef __PLINFO_H__
#define __PLINFO_H__

//#include "strutil.h"

typedef enum PlTypes {
    ptHuman, ptAI
} PlTypes;

typedef struct PlKeys {
    int left;
    int right;
    int jump;
} PlKeys;

typedef struct AIInfo {
    int id;
} AIInfo;

typedef struct PlInfo {
    int color;
    PlTypes type;
    char *name;
    union control {
        PlKeys keys;
        AIInfo ai;
    } control; 
    char *profil;
    int pitch;
} PlInfo;

const PlInfo *getInfo (int idi);
int getPlInfosCount ();

void loadPlInfos ();
void savePlInfos ();

#endif // __PLINFO_H__
