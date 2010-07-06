#ifndef __PLINFO_H__
#define __PLINFO_H__

#include <string>
#include <sys/types.h>

using namespace std;

enum PlTypes {
    PT_Human, PT_AI
};

struct PlKeys {
    int left;
    int right;
    int jump;
};

struct AIInfo {
    int id;
};

struct PlInfo {
    int color;
    PlTypes type;
    string name;
    union {
        PlKeys keys;
        AIInfo ai;
    }; 
    string profil;
    int pitch;
};

size_t plinfo_get_count ();

const PlInfo& plinfo_get (size_t idi);

void plinfo_load ();

void plinfo_save ();

#endif // __PLINFO_H__
