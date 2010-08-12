/* 
 * File:   pl_info.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 15:30
 */

#ifndef PL_INFO_H
#define	PL_INFO_H

#include <glibmm/ustring.h>

#include "pl_type.h"

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
    Glib::ustring name;
    union {
        PlKeys keys;
        AIInfo ai;
    };
    Glib::ustring profil;
    int pitch;

    PlInfo();

    PlInfo (uint32_t nc, const Glib::ustring& nn, int nai, const Glib::ustring& np, int npi);
};

#endif	/* PL_INFO_H */

