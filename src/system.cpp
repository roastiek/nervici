#include <vector>
#include <iostream>
#include <glibmm/miscutils.h>
#include <glibmm/module.h>
#include <glibmm/fileutils.h>

#include "mods/mod_interface.h"

#include "system.h"

using namespace std;
using namespace Glib;

namespace System {

static ustring config_dir;
static vector<ustring> data_dirs;

static vector<ModRunner> mod_runners;
static vector<Mod> mods;
static Module* mod_runner = NULL;
ModInterface* mod = NULL;

void init_paths () {
    config_dir = get_user_config_dir () + "/nervici/";

    data_dirs.push_back (get_user_data_dir () + "/nervici/");

    ustring xdg_data_dirs = Glib::getenv ("XDG_DATA_DIRS");
    xdg_data_dirs = (xdg_data_dirs != "") ? xdg_data_dirs : "/usr/local/share:/usr/share";

    size_t start = 0;
    size_t end;

    end = xdg_data_dirs.find_first_of (':', start);
    while (end != xdg_data_dirs.npos) {
        data_dirs.push_back (xdg_data_dirs.substr (start, end - start) + "/nervici/");
        start = end + 1;
        end = xdg_data_dirs.find_first_of (':', start);
    }
    data_dirs.push_back (xdg_data_dirs.substr (start, xdg_data_dirs.length () - start) + "/nervici/");
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

static void find_mod_runners (const vector<ustring>& files, vector<ModRunner>& runners) {
    cout << __func__ << '\n';

    for (size_t fi = 0; fi < files.size (); fi++) {
        const ustring& file = files[fi];
        ustring suffix = file.substr (file.length () - 3, 3).lowercase ();

        if (suffix.compare (".so") == 0) {
            ModRunner entry;
            entry.filename = file;
            runners.push_back (entry);
        }
    }
}

static void find_scripts (const vector<ustring>& files, vector<ModRunner>& runners,
        vector<Mod>& mods) {
    cout << __func__ << '\n';

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

            cout << ext << '\n';

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
    cout << __func__ << '\n';

    vector<ustring> files;

    for (size_t di = 0; di < data_dirs.size (); di++) {
        ustring dir = data_dirs[di] + "mods/";
        scan_mods_dir (dir, files);
    }

    find_mod_runners (files, mod_runners);
    find_scripts (files, mod_runners, mods);
}

void free_mods () {
}

void load_mod (size_t mid, const ustring& script) {
    cout << __func__ << '\n';

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

void mod_on_game_start (const GameSetting* set) {
    /* if (mod_events.on_game_start != NULL)
         mod_events.on_game_start (*set);*/
}

void mod_on_game_end () {
    /*   if (mod_events.on_game_end != NULL)
           mod_events.on_game_end ();*/
}

void mod_on_timer () {
    /*if (mod_events.on_timer != NULL)
        mod_events.on_timer ();*/
}

void mod_on_death (plid_tu plid) {
    /*if (mod_events.on_death != NULL)
        mod_events.on_death (plid);*/
}

void mod_before_step () {
    /*if (mod_events.before_step != NULL)
        mod_events.before_step ();*/
}

void mod_after_step () {
    /*if (mod_events.after_step != NULL)
        mod_events.after_step ();*/
}

void mod_on_pozi_smile (plid_tu plid, int lvl) {
    /*if (mod_events.on_pozi_smile != NULL)
        mod_events.on_pozi_smile (plid, lvl);*/
}

void mod_on_nega_smile (plid_tu plid, int lvl) {
    /*if (mod_events.on_nega_smile != NULL)
        mod_events.on_nega_smile (plid, lvl);*/
}

void mod_on_fleg_smile (plid_tu plid, int lvl) {
    /*if (mod_events.on_fleg_smile != NULL)
        mod_events.on_fleg_smile (plid, lvl);*/
}

void mod_on_iron_smile (plid_tu plid, int lvl) {
    /*if (mod_events.on_iron_smile != NULL)
        mod_events.on_iron_smile (plid, lvl);*/
}

void mod_on_ham_smile (plid_tu plid, int lvl) {
    /*if (mod_events.on_ham_smile != NULL)
        mod_events.on_ham_smile (plid, lvl);*/
}

void mod_on_killed (plid_tu plid, plid_tu murder) {
    /*if (mod_events.on_killed != NULL)
        mod_events.on_killed (plid, murder);*/
}

void mod_on_kill (plid_tu plid, plid_tu victim) {
    /*if (mod_events.on_kill != NULL)
        mod_events.on_kill (plid, victim);*/
}

void mod_on_wall (plid_tu plid) {
    /*if (mod_events.on_wall != NULL)
        mod_events.on_wall (plid);*/
}

void mod_on_self_death (plid_tu plid) {
    /*if (mod_events.on_self_death != NULL)
        mod_events.on_self_death (plid);*/
}

void mod_on_cleared (plid_tu plid) {
    /*if (mod_events.on_cleared != NULL)
        mod_events.on_cleared (plid);*/
}

void mod_on_pl_timer (plid_tu plid) {
    /*if (mod_events.on_pl_timer != NULL)
        mod_events.on_pl_timer (plid);*/
}
}

