#include <vector>
#include <glibmm/miscutils.h>
#include <glibmm/module.h>
#include <glibmm/fileutils.h>
#include <giomm/file.h>

#include "mods/mod_interface.h"

#include "system.h"

using namespace std;
using namespace Glib;
using namespace Gio;

namespace System {

static ustring config_dir;
static vector<ustring> data_dirs;

static vector<ModRunner> mod_runners;
static vector<Mod> mods;
static Module* mod_runner = NULL;
ModInterface* mod = NULL;

void init_paths () {
    config_dir = get_user_config_dir () + "/nervici/";
    RefPtr<File> dir = File::create_for_path (config_dir);
    try {
        dir->make_directory_with_parents ();
    } catch (Gio::Error) {
    }

    data_dirs.push_back (get_user_data_dir () + "/nervici/");

    const gchar * const *system_data_dirs = g_get_system_data_dirs ();
    for (size_t di = 0; system_data_dirs[di] != NULL; di++) {
        data_dirs.push_back (ustring(system_data_dirs[di]) + "/nervici/");
    }
}

void free_paths () {
}

static void scan_mods_dir (const ustring& path, vector<ustring>& files) {
    try {
        Dir dir (path);
        for (DirIterator it = dir.begin (); it != dir.end (); it++) {
            if ((*it)[0] == '.') continue;
            files.push_back (path + (*it));
        }
    } catch (FileError fe) {
    }
}

#define RUNNER_SUFFIX ".so"

static void find_mod_runners (const vector<ustring>& files, vector<ModRunner>& runners) {
    for (size_t fi = 0; fi < files.size (); fi++) {
        const ustring& file = files[fi];
        ustring suffix = file.substr (file.length () - 3, 3).lowercase ();

        if (suffix.compare (RUNNER_SUFFIX) == 0) {
            ModRunner entry;
            entry.filename = file;
            runners.push_back (entry);
        }
    }
}

static void find_scripts (const vector<ustring>& files, 
        vector<ModRunner>& runners, vector<Mod>& mods) {

    for (size_t ri = 0; ri < runners.size (); ri++) {
        ModRunner& runner = runners[ri];
        Mod entry;
        const ModInfo* minfo;

        Module mod_lib (runner.filename);
        if (!mod_lib) continue;

        GetFaceHandle get_face;
        if (!mod_lib.get_symbol ("get_face", get_face.handle)) continue;

        ModInterface* face = get_face.get_face ();
        const ModRunnerInfo& rinfo = face->get_runner_info ();

        for (size_t ei = 0; ei < rinfo.extensions.size (); ei++) {
            ustring ext = rinfo.extensions[ei].lowercase ();
            size_t ext_len = ext.length ();

            for (size_t fi = 0; fi < files.size (); fi++) {
                const ustring& script = files[fi];
                ustring suffix = script.substr (script.length () - ext_len, ext_len);

                if (ext.compare (suffix) != 0) continue;

                minfo = face->get_info (script);
                if (minfo == NULL) continue;

                entry.runner = &runner;
                entry.script = script;
                entry.autor = minfo->autor;
                entry.name = minfo->name;
                entry.rules = minfo->rules;
                entry.spec = minfo->spec;
                mods.push_back (entry);
            }
        }

        minfo = face->get_info ("");
        if (minfo != NULL) {
            entry.runner = &runner;
            entry.script = "";
            entry.autor = minfo->autor;
            entry.name = minfo->name;
            entry.rules = minfo->rules;
            entry.spec = minfo->spec;
            mods.push_back (entry);
        }
        delete face;
    }
}

void find_mods () {
    vector<ustring> files;

    for (size_t di = 0; di < data_dirs.size (); di++) {
        ustring dir = data_dirs[di] + "mods/";
        scan_mods_dir (dir, files);
    }

    find_mod_runners (files, mod_runners);
    find_scripts (files, mod_runners, mods);
}

void free_mods () {
    mods.clear ();
    mod_runners.clear ();
}

void load_mod (size_t mid, const ustring& script) {
    mod_runner = new Module (mod_runners[mid].filename);
    GetFaceHandle get_face;
    mod_runner->get_symbol ("get_face", get_face.handle);
    mod = get_face.get_face();
    mod->load_script (script);
}

void unload_mod () {
    delete mod;
    mod = NULL;
    delete mod_runner;
    mod_runner = NULL;
}

const ustring& get_config_dir () {
    return config_dir;
}

size_t get_data_dirs_count () {
    return data_dirs.size ();
}

const ustring& get_data_dir (size_t index) {
    return data_dirs[index];
}

size_t get_mods_count () {
    return mods.size ();
}

const Mod& get_mod (size_t mid) {
    return mods[mid];
}

}

