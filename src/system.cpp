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

#include "config.h"
#include "mods/mods.h"

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

#define PROFILE_DIR "/.nervici"
#define PROFILE_FILE "/nervici.conf"
#define HOME_MODS_DIR "/mods"
#define HOME_FONTS_DIR "/fonts"
#define HOME_SOUNDS_DIR "/sounds"
#define HOME_MUSIC_DIR "/music"

static ustring home_dir;
static ustring profile_dir;
static ustring profile_file;
static ustring mods_dir;
static ustring mods_dir_home;
static ustring images_dir;
static ustring fonts_dir;
static ustring fonts_dir_home;
static ustring sounds_dir;
static ustring sounds_dir_home;
static ustring music_dir;
static ustring music_dir_home;
static vector<ModRunner> mod_runners;
static vector<Mod> mods;
static ModEvents mod_events;
static void* mod_handle;

static ustring resolv_home_dir () {
    return getenv ("HOME");
}

static ustring resolv_profile_dir () {
    return home_dir + PROFILE_DIR;
}

static ustring resolv_profile_file () {
    return profile_dir + PROFILE_FILE;
}

static ustring resolv_dir (const ustring& dir) {
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

static ustring resolv_dir_home (const ustring& dir) {
    return profile_dir + dir;
}

void init_paths () {
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

void find_mods () {
    cout << __func__ << '\n';

    vector<ustring> files;

    scan_mods_dir (mods_dir, files);
    scan_mods_dir (mods_dir_home, files);

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

const ustring & get_profile_file () {
    return profile_file;
}

const ustring & get_images_dir () {
    return images_dir;
}

const ustring & get_fonts_dir () {
    return fonts_dir;
}

const ustring & get_fonts_dir_home () {
    return fonts_dir_home;
}

const ustring & get_sounds_dir () {
    return sounds_dir;
}

const ustring & get_sounds_dir_home () {
    return sounds_dir_home;
}

const ustring & get_music_dir () {
    return music_dir;
}

const ustring & get_music_dir_home () {
    return music_dir_home;
}

size_t get_mods_count () {
    return mods.size ();
}

const Mod & get_mod (size_t mid) {
    return mods[mid];
}

void mod_on_game_start (const GameSetting * set) {
    if (mod_events.on_game_start != NULL)
        mod_events.on_game_start (set);
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

void mod_on_pozi_smile (int smid, int lvl) {
    if (mod_events.on_pozi_smile != NULL)
        mod_events.on_pozi_smile (smid, lvl);
}

void mod_on_nega_smile (int smid, int lvl) {
    if (mod_events.on_nega_smile != NULL)
        mod_events.on_nega_smile (smid, lvl);
}

void mod_on_fleg_smile (int smid, int lvl) {
    if (mod_events.on_fleg_smile != NULL)
        mod_events.on_fleg_smile (smid, lvl);
}

void mod_on_iron_smile (int smid, int lvl) {
    if (mod_events.on_iron_smile != NULL)
        mod_events.on_iron_smile (smid, lvl);
}

void mod_on_ham_smile (int smid, int lvl) {
    if (mod_events.on_ham_smile != NULL)
        mod_events.on_ham_smile (smid, lvl);
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