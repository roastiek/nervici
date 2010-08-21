/* 
 * File:   mod.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 19:39
 */

#ifndef MOD_H
#define	MOD_H

#include "mods/mod_specification.h"
#include "fakes/mod_runner.h"

struct Mod {
    const ModRunner& runner;
    Glib::ustring script;
    Glib::ustring name;
    Glib::ustring autor;
    Glib::ustring rules;
    ModSpecification spec;

    Mod (const ModRunner& nrunner, const Glib::ustring& nscript,
            const Glib::ustring& nname, const Glib::ustring& nautor,
            const Glib::ustring& nrules, const ModSpecification& nspec) :
        runner (nrunner), script (nscript), name (nname), autor (nautor),
                rules (nrules), spec (nspec) {

    }
};

#endif	/* MOD_H */

