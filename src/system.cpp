#include <dirent.h>
//#include <stdio.h>
//#include <unistd.h>
//#include <stdlib.h>
#include <string.h>
//#include <strings.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>
using namespace std;
#include <glibmm/miscutils.h>
using namespace Glib;


//#include "config.h"
#include "mods/mod_interface.h"

#include "system.h"

namespace System {

struct ModEvents {
    LoadScript load_script;
    UnloadScript unload_script;
    ModOnGameStart on_game_start;
    ModOnGameEnd on_game_end;
    ModOnTimer on_timer;
    ModOnDeath on_death;
    ModBeforeStep before_step;
    ModAfterStep after_step;
    ModOnPoziSmile on_pozi_smile;
    ModOnNegaSmile on_nega_smile;
    ModOnFlegSmile on_fleg_smile;
    ModOnIronSmile on_iron_smile;
    ModOnHamSmile on_ham_smile;
    ModOnKilled on_killed;
    ModOnKill on_kill;
    ModOnWall on_wall;
    ModOnSelfDeath on_self_death;
    ModOnCleared on_cleared;
    ModOnPlTimer on_pl_timer;
};

static ustring config_dir;
static vector<ustring> data_dirs;

static vector<ModRunner> mod_runners;
static vector<Mod> mods;
static ModEvents mod_events;
static void* mod_handle;

void init_paths () {
    config_dir = get_user_config_dir () + "/nervici/";

    data_dirs.push_back (get_user_data_dir () + "/nervici/");

    ustring xdg_data_dirs = Glib::getenv ("XDG_DATA_DIRS");
    xdg_data_dirs = (xdg_data_dirs != "") ? xdg_data_dirs : "/usr/local/share:/usr/share";

    size_t start = 0;
    size_t end;
    
    end = xdg_data_dirs.find_first_of (':', start);
    while (end != -1) {
        data_dirs.push_back (xdg_data_dirs.substr (start, end - start) + "/nervici/");
        start = end + 1;
        end = xdg_data_dirs.find_first_of (':', start);
    }
    data_dirs.push_back (xdg_data_dirs.substr (start, xdg_data_dirs.length () - start) + "/nervici/");

    for (size_t i = 0; i < data_dirs.size (); i++) {
        cout << data_dirs[i] << '\n';
    }
}

void free_paths () {
}

static void scan_mods_dir (const ustring& path, vector<ustring>& files) {
    DIR *dir;

    dir = opendir (path.c_str ());
    if (dir == NULL) return;

    for (struct dirent *ent = readdir (dir); ent != NULL; ent = readdir (dir)) {
        if (ent->d_name[0] == '.') continue;
        files.push_back (path + '/' + ent->d_name);
    }
}

static void find_mod_runners (const vector<ustring>& files, vector<ModRunner>& runners) {
    cout << __func__ << '\n';

    for (size_t fi = 0; fi < files.size (); fi++) {
        const ustring& file = files[fi];
        ustring suffix = file.substr (file.length () - 3, 3);

        if (strcasecmp (".so", suffix.c_str ()) == 0) {
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
        long int fake;
        Mod entry;
        const ModInfo* minfo;

        void* handle = dlopen (runner.filename.c_str (), RTLD_NOW);
        if (handle == NULL) continue;

        fake = (long int) dlsym (handle, "get_mod_runner_info");
        ModGetModRunnerInfo get_mri = (ModGetModRunnerInfo) fake;
        if (get_mri == NULL) continue;

        fake = (long int) dlsym (handle, "get_mod_info");
        ModGetModInfo get_mi = (ModGetModInfo) fake;
        if (get_mi == NULL) continue;

        const ModRunnerInfo* rinfo = get_mri ();
        if (rinfo == NULL) continue;

        if (rinfo->extensions != NULL) {
            for (size_t ei = 0; rinfo->extensions[ei] != NULL; ei++) {
                const char* ext = rinfo->extensions[ei];
                size_t ext_len = strlen (ext);

                for (size_t fi = 0; fi < files.size (); fi++) {
                    const string& script = files[fi];
                    string suffix = script.substr (script.length () - ext_len, ext_len);

                    if (strcasecmp (ext, suffix.c_str ()) != 0) continue;

                    minfo = get_mi (script.c_str ());
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
        }

        minfo = get_mi (NULL);
        if (minfo != NULL) {
            entry.runner = &runner;
            entry.script = "";
            entry.autor = minfo->autor;
            entry.name = minfo->name;
            entry.rules = minfo->rules;
            entry.spec = minfo->spec;
            mods.push_back (entry);
        }
        dlclose (handle);
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
    /*    scan_mods_path (mods_dir, mod_runners);
        scan_mods_path (mods_dir_home, mod_runners);*/
}

void free_mods () {
}

void load_mod (size_t mid, const ustring& script) {
    long int fake;

    cout << __func__ << '\n';

    mod_handle = dlopen (mod_runners[mid].filename.c_str (), RTLD_NOW);
    if (!mod_handle) return;


    fake = (long int) dlsym (mod_handle, "load_script");
    mod_events.load_script = (LoadScript) fake;

    fake = (long int) dlsym (mod_handle, "unload_script");
    mod_events.unload_script = (UnloadScript) fake;

    fake = (long int) dlsym (mod_handle, "after_step");
    mod_events.after_step = (ModAfterStep) fake;

    fake = (long int) dlsym (mod_handle, "before_step");
    mod_events.before_step = (ModBeforeStep) fake;

    fake = (long int) dlsym (mod_handle, "on_cleared");
    mod_events.on_cleared = (ModOnCleared) fake;

    fake = (long int) dlsym (mod_handle, "on_death");
    mod_events.on_death = (ModOnDeath) fake;

    fake = (long int) dlsym (mod_handle, "on_fleg_smile");
    mod_events.on_fleg_smile = (ModOnFlegSmile) fake;

    fake = (long int) dlsym (mod_handle, "on_game_end");
    mod_events.on_game_end = (ModOnGameEnd) fake;

    fake = (long int) dlsym (mod_handle, "on_game_start");
    mod_events.on_game_start = (ModOnGameStart) fake;

    fake = (long int) dlsym (mod_handle, "on_ham_smile");
    mod_events.on_ham_smile = (ModOnHamSmile) fake;

    fake = (long int) dlsym (mod_handle, "on_iron_smile");
    mod_events.on_iron_smile = (ModOnIronSmile) fake;

    fake = (long int) dlsym (mod_handle, "on_kill");
    mod_events.on_kill = (ModOnKill) fake;

    fake = (long int) dlsym (mod_handle, "on_killed");
    mod_events.on_killed = (ModOnKilled) fake;

    fake = (long int) dlsym (mod_handle, "on_nega_smile");
    mod_events.on_nega_smile = (ModOnNegaSmile) fake;

    fake = (long int) dlsym (mod_handle, "on_pl_timer");
    mod_events.on_pl_timer = (ModOnPlTimer) fake;

    fake = (long int) dlsym (mod_handle, "on_pozi_smile");
    mod_events.on_pozi_smile = (ModOnPoziSmile) fake;

    fake = (long int) dlsym (mod_handle, "on_self_death");
    mod_events.on_self_death = (ModOnSelfDeath) fake;

    fake = (long int) dlsym (mod_handle, "on_timer");
    mod_events.on_timer = (ModOnTimer) fake;

    fake = (long int) dlsym (mod_handle, "on_wall");
    mod_events.on_wall = (ModOnWall) fake;

    mod_events.load_script (script.c_str ());
}

void unload_mod () {
    mod_events.unload_script ();
    dlclose (mod_handle);
}

const ustring& get_config_dir () {
    return config_dir;
}

const vector<ustring>& get_data_dirs () {
    return data_dirs;
}

size_t get_mods_count () {
    return mods.size ();
}

const Mod & get_mod (size_t mid) {
    return mods[mid];
}

void mod_on_game_start (const GameSetting * set) {
    if (mod_events.on_game_start != NULL)
        mod_events.on_game_start (*set);
}

void mod_on_game_end () {
    if (mod_events.on_game_end != NULL)
        mod_events.on_game_end ();
}

void mod_on_timer () {
    if (mod_events.on_timer != NULL)
        mod_events.on_timer ();
}

void mod_on_death (plid_tu plid) {
    if (mod_events.on_death != NULL)
        mod_events.on_death (plid);
}

void mod_before_step () {
    if (mod_events.before_step != NULL)
        mod_events.before_step ();
}

void mod_after_step () {
    if (mod_events.after_step != NULL)
        mod_events.after_step ();
}

void mod_on_pozi_smile (plid_tu plid, int lvl) {
    if (mod_events.on_pozi_smile != NULL)
        mod_events.on_pozi_smile (plid, lvl);
}

void mod_on_nega_smile (plid_tu plid, int lvl) {
    if (mod_events.on_nega_smile != NULL)
        mod_events.on_nega_smile (plid, lvl);
}

void mod_on_fleg_smile (plid_tu plid, int lvl) {
    if (mod_events.on_fleg_smile != NULL)
        mod_events.on_fleg_smile (plid, lvl);
}

void mod_on_iron_smile (plid_tu plid, int lvl) {
    if (mod_events.on_iron_smile != NULL)
        mod_events.on_iron_smile (plid, lvl);
}

void mod_on_ham_smile (plid_tu plid, int lvl) {
    if (mod_events.on_ham_smile != NULL)
        mod_events.on_ham_smile (plid, lvl);
}

void mod_on_killed (plid_tu plid, plid_tu murder) {
    if (mod_events.on_killed != NULL)
        mod_events.on_killed (plid, murder);
}

void mod_on_kill (plid_tu plid, plid_tu victim) {
    if (mod_events.on_kill != NULL)
        mod_events.on_kill (plid, victim);
}

void mod_on_wall (plid_tu plid) {
    if (mod_events.on_wall != NULL)
        mod_events.on_wall (plid);
}

void mod_on_self_death (plid_tu plid) {
    if (mod_events.on_self_death != NULL)
        mod_events.on_self_death (plid);
}

void mod_on_cleared (plid_tu plid) {
    if (mod_events.on_cleared != NULL)
        mod_events.on_cleared (plid);
}

void mod_on_pl_timer (plid_tu plid) {
    if (mod_events.on_pl_timer != NULL)
        mod_events.on_pl_timer (plid);
}
}
