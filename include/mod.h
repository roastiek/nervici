/* 
 * File:   mod.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 19:39
 */

#ifndef MOD_H
#define	MOD_H

#include "mod_runner.h"
#include "mods/mod_specification.h"

struct Mod {
    const ModRunner *runner;
    Glib::ustring script;
    Glib::ustring name;
    Glib::ustring autor;
    Glib::ustring rules;
    ModSpecification spec;
};


#endif	/* MOD_H */

