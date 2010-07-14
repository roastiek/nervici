/* 
 * File:   setting_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 21:04
 */

#ifndef SETTING_DEFS_H
#define	SETTING_DEFS_H

#include <glibmm/ustring.h>

using namespace Glib;

struct SetEntry {
    SetEntry *next;
    ustring key;
    ustring value;
};

struct SetSection {
    SetSection *next;
    ustring name;
    SetEntry *first_entry;
};


#endif	/* SETTING_DEFS_H */

