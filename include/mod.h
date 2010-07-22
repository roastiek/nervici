/* 
 * File:   mod.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 19:39
 */

#ifndef MOD_H
#define	MOD_H

#include "mod_runner.h"

struct Mod {
    const ModRunner *runner;
    ustring script;
    ustring name;
    ustring autor;
    ustring rules;
};


#endif	/* MOD_H */

