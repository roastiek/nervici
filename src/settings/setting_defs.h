/* 
 * File:   setting_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 21:04
 */

#ifndef SETTING_DEFS_H
#define	SETTING_DEFS_H

#include <string>

using namespace std;

struct SetEntry {
    SetEntry *next;
    string key;
    string value;
};

struct SetSection {
    SetSection *next;
    string name;
    SetEntry *first_entry;
};


#endif	/* SETTING_DEFS_H */

