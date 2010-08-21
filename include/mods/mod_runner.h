/* 
 * File:   mod_runner.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 19:39
 */

#ifndef MOD_RUNNER_H
#define	MOD_RUNNER_H

#include <glibmm/ustring.h>

#include "get_face.h"

struct ModRunner {
    Glib::ustring filename;
    GetFace get_face;

    ModRunner (const Glib::ustring& nfilename, GetFace face) :
        filename (nfilename), get_face (face) {

    }
};

#endif	/* MOD_RUNNER_H */

