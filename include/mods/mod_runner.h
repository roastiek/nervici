/* 
 * File:   mod_runner.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 19:39
 */

#ifndef MOD_RUNNER_H
#define	MOD_RUNNER_H

#include "get_face.h"
#include "fakes/glibmm_decl.h"

struct ModRunner {
    Glib::ustring filename;
    GetFace get_face;

    ModRunner (const Glib::ustring& nfilename, GetFace face) :
        filename (nfilename), get_face (face) {

    }
};

#endif	/* MOD_RUNNER_H */

