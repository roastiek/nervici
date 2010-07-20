/* 
 * File:   plinfo_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 17:31
 */

#ifndef PLINFO_DEFS_H
#define	PLINFO_DEFS_H

#include <glibmm/ustring.h>

using namespace Glib;

#define AIS_COUNT 16

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
    uint32_t color;
    PlTypes type;
    ustring name;
    union {
        PlKeys keys;
        AIInfo ai;
    };
    ustring profil;
    int pitch;

    PlInfo();

    PlInfo (uint32_t nc, const ustring& nn, int nai, const ustring& np, int npi);
};

#endif	/* PLINFO_DEFS_H */

