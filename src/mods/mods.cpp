#include <glibmm/fileutils.h>
#include <glibmm/module.h>

#include "system.h"
#include "logger.h"
#include "mods/mod_runner.h"
#include "mods/mod.h"
#include "mods/mod_interface.h"
#include "mods/get_face.h"
#include "mods/standard_mods.h"
#include "mods/mod_runner_info.h"
#include "mods/mod_info.h"

#include "mods/mods.h"

using namespace std;
using namespace Glib;

union GetFaceHandle {
    void* handle;
    GetFace get_face;
};

Mods Mods::instance;

Mods& mods = Mods::get_instance ();

Mods::Mods () :
    mod_module (NULL), mod (NULL) {

}

Mods::~Mods () {
    logger.fineln ("freeing mods");

    for (size_t mi = 0; mi < mods.size (); mi++) {
        delete mods[mi];
    }
    mods.clear ();

    for (size_t mi = 0; mi < mod_runners.size (); mi++) {
        delete mod_runners[mi];
    }
    mod_runners.clear ();

}

void Mods::find_mods () {
    logger.fineln ("searching for mods");

    vector<ustring> files;
    ustring dir;

    dir = paths.get_data_dir () + "mods/";
    scan_mods_dir (dir, files);

    if (paths.check_user_dir ()) {
        dir = paths.get_user_data_dir () + "mods/";
        scan_mods_dir (dir, files);
    }

    find_mod_runners (files);
    find_scripts (files);
}

void Mods::load_mod (size_t mid) {
    logger.fineln ("loading mod \"%s\"", mods[mid]->id.c_str ());

    GetFaceHandle handle;
    const ModRunner& runner = mods[mid]->runner;
    if (runner.get_face == NULL) {
        mod_module = new Module (runner.filename);
        mod_module->get_symbol ("get_face", handle.handle);
    } else {
        handle.get_face = runner.get_face;
    }
    mod = handle.get_face ();
    mod->load_script (mods[mid]->script);
}

void Mods::unload_mod () {
    logger.fineln ("unloading mod");

    if (mod != NULL) {
        delete mod;
        mod = NULL;
    }
    if (mod_module != NULL) {
        delete mod_module;
        mod_module = NULL;
    }
}

void Mods::scan_mods_dir (const ustring& path, vector<ustring>& files) {
    logger.fineln ("searching for mods in %s", path.c_str ());
    try {
        Dir dir (path);
        for (DirIterator it = dir.begin (); it != dir.end (); it++) {
            if ((*it)[0] == '.')
                continue;
            files.push_back (path + (*it));
        }
    } catch (FileError fe) {
    }
}

#define RUNNER_SUFFIX ".so"

void Mods::find_mod_runners (const vector<ustring>& files) {
    mod_runners.push_back (new ModRunner ("", get_cervici_face));

    for (size_t fi = 0; fi < files.size (); fi++) {
        const ustring& file = files[fi];
        ustring suffix = file.substr (file.length () - 3, 3).lowercase ();

        if (suffix.compare (RUNNER_SUFFIX) == 0) {
            mod_runners.push_back (new ModRunner (file, NULL));
            logger.fineln ("found mod runner %s", file.c_str ());
        }
    }
}

void Mods::find_scripts (const vector<ustring>& files) {

    for (size_t ri = 0; ri < mod_runners.size (); ri++) {
        ModRunner& runner = *mod_runners[ri];
        const ModInfo* minfo;
        GetFaceHandle handle;

        if (runner.get_face == NULL) {

            Module mod_lib (runner.filename);
            if (!mod_lib)
                continue;

            if (!mod_lib.get_symbol ("get_face", handle.handle))
                continue;

        } else {
            handle.get_face = runner.get_face;
        }

        ModInterface* face = handle.get_face ();
        const ModRunnerInfo* rinfo = face->get_runner_info ();
        if (rinfo == NULL)
            continue;

        for (size_t ei = 0; rinfo->extensions[ei] != NULL; ei++) {
            ustring ext = ustring (rinfo->extensions[ei]).lowercase ();
            size_t ext_len = ext.length ();

            for (size_t fi = 0; fi < files.size (); fi++) {
                const ustring& script = files[fi];
                ustring suffix = script.substr (script.length () - ext_len,
                    ext_len);

                if (ext.compare (suffix) != 0)
                    continue;

                minfo = face->get_info (script);
                if (minfo == NULL)
                    continue;

                mods.push_back (new Mod (runner,
                    script,
                    minfo->id,
                    minfo->name,
                    minfo->autor,
                    minfo->rules,
                    minfo->spec));
                logger.fineln ("found mod %s", minfo->id);
            }
        }

        minfo = face->get_info ("");
        if (minfo != NULL) {
            mods.push_back (new Mod (runner,
                "",
                minfo->id,
                minfo->name,
                minfo->autor,
                minfo->rules,
                minfo->spec));
            logger.fineln ("found mod %s", minfo->id);
        }
        delete face;
    }
}

