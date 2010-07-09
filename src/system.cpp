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

#include "config.h"
#include "system.h"

using namespace std;

#define PROFILE_DIR "/.nervici"
#define PROFILE_FILE "/nervici.conf"
#define HOME_MODS_DIR "/mods"
#define HOME_FONTS_DIR "/fonts"
#define HOME_SOUNDS_DIR "/sounds"
#define HOME_MUSIC_DIR "/music"

string System::home_dir;
string System::profile_dir;
string System::profile_file;
string System::mods_dir;
string System::mods_dir_home;
string System::images_dir;
string System::fonts_dir;
string System::fonts_dir_home;
string System::sounds_dir;
string System::sounds_dir_home;
string System::music_dir;
string System::music_dir_home;
vector<ModRunner> System::mod_runners;
vector<Mod> System::mods;
ModEvents System::mod_events;
void *System::mod_handle;

string System::resolv_home_dir () {
    return getenv ("HOME");
}

string System::resolv_profile_dir () {
    return home_dir + PROFILE_DIR;
}

string System::resolv_profile_file () {
    return profile_dir + PROFILE_FILE;
}

string System::resolv_dir (const string& dir) {
    string result;

#ifdef USE_WORKING_DIR

    char *cwd = getcwd (NULL, 0);
    result = cwd + dir;
    free (cwd);

#else

    result = NERVICI_PREFIX + dir;

#endif

    return result;
}

string System::resolv_dir_home (const string& dir) {
    return profile_dir + dir;
}

void System::init_paths () {
    home_dir = resolv_home_dir ();
    profile_dir = resolv_profile_dir ();
    profile_file = resolv_profile_file ();

    mods_dir = resolv_dir (NERVICI_MODS_DIR);
    mods_dir_home = resolv_dir_home (HOME_MODS_DIR);
    images_dir = resolv_dir (NERVICI_IMAGES_DIR);
    fonts_dir = resolv_dir (NERVICI_FONTS_DIR);
    fonts_dir_home = resolv_dir_home (HOME_FONTS_DIR);
    sounds_dir = resolv_dir (NERVICI_SOUNDS_DIR);
    sounds_dir_home = resolv_dir_home (HOME_SOUNDS_DIR);
    music_dir = resolv_dir (NERVICI_MUSIC_DIR);
    music_dir_home = resolv_dir_home (HOME_MUSIC_DIR);

    mkdir (profile_dir.c_str (), 0755);
    mkdir (mods_dir_home.c_str (), 0755);
    mkdir (fonts_dir_home.c_str (), 0755);
    mkdir (sounds_dir_home.c_str (), 0755);
    mkdir (music_dir_home.c_str (), 0755);
}

void System::free_paths () {
}

static void scan_mods_dir (const string& path, vector<string>& files) {
    DIR *dir;

    dir = opendir (path.c_str ());
    if (dir == NULL) return;

    for (struct dirent *ent = readdir (dir); ent != NULL; ent = readdir (dir)) {
        if (ent->d_name[0] == '.') continue;
        files.push_back (path + '/' + ent->d_name);
    }
}

static void find_mod_runners (const vector<string>& files, vector<ModRunner>& runners) {
    cout << __func__ << '\n';

    for (size_t fi = 0; fi < files.size (); fi++) {
        const string& file = files[fi];
        string suffix = file.substr (file.length () - 3, 3);

        if (strcasecmp (".so", suffix.c_str ()) == 0) {
            ModRunner entry;
            entry.filename = file;
            runners.push_back (entry);
        }
    }
}

static void find_scripts (const vector<string>& files, vector<ModRunner>& runners,
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
            mods.push_back (entry);
        }
        dlclose (handle);
    }
}

void System::find_mods () {
    cout << __func__ << '\n';

    vector<string> files;

    scan_mods_dir (mods_dir, files);
    scan_mods_dir (mods_dir_home, files);

    find_mod_runners (files, mod_runners);
    find_scripts (files, mod_runners, mods);
    /*    scan_mods_path (mods_dir, mod_runners);
        scan_mods_path (mods_dir_home, mod_runners);*/
}

void System::free_mods () {
}

void System::load_mod (size_t mid, const string& script) {
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

void System::unload_mod () {
    mod_events.unload_script ();
    dlclose (mod_handle);
}

