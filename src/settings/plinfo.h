#ifndef __PLINFO_H__
#define __PLINFO_H__

#include <sys/types.h>

typedef enum _PlTypes {
    PT_Human, PT_AI
} PlTypes;

typedef struct _PlKeys {
    int left;
    int right;
    int jump;
} PlKeys;

typedef struct _AIInfo {
    int id;
} AIInfo;

typedef struct _PlInfo {
    int color;
    PlTypes type;
    char *name;
    union {
        PlKeys keys;
        AIInfo ai;
    } control; 
    char *profil;
    int pitch;
} PlInfo;

size_t plinfo_get_count ();
const PlInfo *plinfo_get (size_t idi);

void plinfo_load ();
void plinfo_save ();

#endif // __PLINFO_H__
