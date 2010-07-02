#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dlfcn.h>
#include <sys/stat.h>

#include "config.h"
#include "system.h"
#include "utils.h"

#define PROFILE_DIR "/.nervici"
#define PROF_FILE "/nervici.conf"
#define HOME_MODS_DIR "/mods"
#define HOME_FONTS_DIR "/fonts"
#define HOME_SOUNDS_DIR "/sounds"
#define HOME_MUSIC_DIR "/music"

typedef struct ModEventEntries {
    pro_onGameStart onGameStart;
    pro_onGameEnd onGameEnd;
    pro_onTimer onTimer;
    pro_onDeath onDeath;
    pro_beforeStep beforeStep;
    pro_afterStep afterStep;
    pro_onPoziSmile onPoziSmile;
    pro_onNegaSmile onNegaSmile;
    pro_onFlegSmile onFlegSmile;
    pro_onIronSmile onIronSmile;
    pro_onHamSmile onHamSmile;
    pro_onKilled onKilled;
    pro_onKill onKill;
    pro_onWall onWall;
    pro_onSelfDeath onSelfDeath;
    pro_onCleared onCleared;
    pro_onPlTimer onPlTimer;
} ModEventEntries;

static char *home_dir = NULL;
static char *profile_dir = NULL;
static char *profile = NULL;
static char *mods_dir = NULL;
static char *mods_dir_home = NULL;
static char *images_dir = NULL;
static char *fonts_dir = NULL;
static char *fonts_dir_home = NULL;
static char *sounds_dir = NULL;
static char *sounds_dir_home = NULL;
static char *music_dir = NULL;
static char *music_dir_home = NULL;

typedef struct {
    size_t count;
    ModEntry *items;
} ModEntries;

static ModEntries mod_entries = {0, NULL};

static ModEventEntries modevent;
static void *modhandle;

static char *resolv_home_dir () {
    return str_copy (getenv ("HOME"));
}

static char *resolv_profile_dir () {
    return str_concat (home_dir, PROFILE_DIR);
}

static char *resolv_profile_file () {
    return str_concat (profile_dir, PROF_FILE);
}

static char *resolv_dir (const char *dir) {
    char * result;
    
    #ifdef USE_WORKING_DIR

    char *cwd = getcwd (NULL, 0);
    result = str_concat (cwd, dir);
    free (cwd);

    #else

    result = str_concat (NERVICI_PREFIX, dir);

    #endif
    
    return result;
}

static char * resolv_dir_home (const char *dir) {
    return str_concat (profile_dir, dir);
}

void sys_init_paths () {
    home_dir = resolv_home_dir ();
    profile_dir = resolv_profile_dir ();
    profile = resolv_profile_file ();

    mods_dir = resolv_dir (NERVICI_MODS_DIR);
    mods_dir_home = resolv_dir_home (HOME_MODS_DIR);
    images_dir = resolv_dir (NERVICI_IMAGES_DIR);
    fonts_dir = resolv_dir (NERVICI_FONTS_DIR);
    fonts_dir_home = resolv_dir_home (HOME_FONTS_DIR);
    sounds_dir = resolv_dir (NERVICI_SOUNDS_DIR);
    sounds_dir_home = resolv_dir_home (HOME_SOUNDS_DIR);
    music_dir = resolv_dir (NERVICI_MUSIC_DIR);
    music_dir_home = resolv_dir_home (HOME_MUSIC_DIR);

    mkdir (profile_dir, 0755);
    mkdir (mods_dir_home, 0755);
    mkdir (fonts_dir_home, 0755);
    mkdir (sounds_dir_home, 0755);
    mkdir (music_dir_home, 0755);
}

void sys_free_paths () {
    free (home_dir);
    free (profile_dir);
    free (profile);
    free (mods_dir);
    free (mods_dir_home);
    free (images_dir);
    free (fonts_dir);
    free (fonts_dir_home);
    free (sounds_dir);
    free (sounds_dir_home);
    free (music_dir);
    free (music_dir_home);
}

static ModInfo *load_mod_info (const char * filename) {
    void *handle;
    ModInfo *result = NULL;
    
    handle = dlopen (filename, RTLD_NOW);
    if (!handle) {
        printf ("load mod  info %s\n", dlerror ());
        return NULL;
    }
    
    pro_getModInfo getMI;
    long int fake = (long int) dlsym (handle, "getModInfo");
    getMI = (pro_getModInfo) fake;
    if (getMI != NULL) {
        result = getMI ();
    }
    dlclose (handle);

    return result;
}

static void scan_mods_path (const char *path, ModEntries *entries) {
    DIR *dir;
    struct dirent *ent;
    const char *suffix;
    ModInfo *info;
    char *mod_file;
    ModEntry entry;

    dir = opendir (path);
    if (dir == NULL) return;
    
    for (ent = readdir (dir); ent != NULL; ent = readdir (dir)) {
        if (ent->d_name[0] == '.') continue;
        suffix = strrchr (ent->d_name, '.');
        if (suffix == NULL) continue;
        if (strcasecmp (suffix, ".so") != 0) continue;    

        mod_file = str_concat_path (path, ent->d_name);

        info = load_mod_info (mod_file);
        if (info != NULL) {
            entry.filename = mod_file;
            entry.info = info;
            array_append (*entries, entry);
        } else {
            free (mod_file);
        }
        
    }
    
    closedir (dir);
}

void sys_find_mods () {
    scan_mods_path (mods_dir, &mod_entries);
    scan_mods_path (mods_dir_home, &mod_entries);
}

void sys_free_mods () {
    if (mod_entries.count > 0) {
        for (int e = 0; e < mod_entries.count; e++) {
            free (mod_entries.items[e].filename);
            free (mod_entries.items[e].info);
        }
        free (mod_entries.items);
    }
}

size_t sys_get_mods_count () {
    return mod_entries.count;
}

const ModEntry *sys_get_mod (size_t mid) {
    return &mod_entries.items[mid];
}

void sys_load_mod (size_t mid) {
    long int fake;
    
    modhandle = dlopen (mod_entries.items[mid].filename, RTLD_NOW);
    if (!modhandle) return;
    
    printf ("load mod\n");
    
    fake = (long int) dlsym (modhandle, "afterStep");
    modevent.afterStep = (pro_afterStep) fake;

    printf ("load mod %d\n", mod_entries.count);

    fake = (long int) dlsym (modhandle, "beforeStep");
    modevent.beforeStep = (pro_beforeStep) fake;

    fake = (long int) dlsym (modhandle, "onCleared");
    modevent.onCleared = (pro_onCleared) fake;

    fake = (long int) dlsym (modhandle, "onDeath");
    modevent.onDeath = (pro_onDeath) fake;

    fake = (long int) dlsym (modhandle, "onFlegSmile");
    modevent.onFlegSmile = (pro_onFlegSmile) fake;

    fake = (long int) dlsym (modhandle, "onGameEnd");
    modevent.onGameEnd = (pro_onGameEnd) fake;

    fake = (long int) dlsym (modhandle, "onGameStart");
    modevent.onGameStart = (pro_onGameStart) fake;

    fake = (long int) dlsym (modhandle, "onHamSmile");
    modevent.onHamSmile = (pro_onHamSmile) fake;

    fake = (long int) dlsym (modhandle, "onIronSmile");
    modevent.onIronSmile = (pro_onIronSmile) fake;

    fake = (long int) dlsym (modhandle, "onKill");
    modevent.onKill = (pro_onKill) fake;

    fake = (long int) dlsym (modhandle, "onKilled");
    modevent.onKilled = (pro_onKilled) fake;

    fake = (long int) dlsym (modhandle, "onNegaSmile");
    modevent.onNegaSmile = (pro_onNegaSmile) fake;

    fake = (long int) dlsym (modhandle, "onPlTimer");
    modevent.onPlTimer = (pro_onPlTimer) fake;

    fake = (long int) dlsym (modhandle, "onPoziSmile");
    modevent.onPoziSmile = (pro_onPoziSmile) fake;

    fake = (long int) dlsym (modhandle, "onSelfDeath");
    modevent.onSelfDeath = (pro_onSelfDeath) fake;

    fake = (long int) dlsym (modhandle, "onTimer");
    modevent.onTimer = (pro_onTimer) fake;

    fake = (long int) dlsym (modhandle, "onWall");
    modevent.onWall = (pro_onWall) fake;
}

void sys_unload_mod () {
    dlclose (modhandle);
}

void sys_mod_on_game_start (const GameSetting *set) {
    if (modevent.onGameStart != NULL) 
        modevent.onGameStart (set);
}

void sys_mod_on_game_end () {
    if (modevent.onGameEnd != NULL)
        modevent.onGameEnd ();
}

void sys_mod_on_timer () {
    if (modevent.onTimer != NULL)
        modevent.onTimer ();
}

void sys_mod_on_death (int plid) {
    if (modevent.onDeath != NULL)
        modevent.onDeath (plid);
}

void sys_mod_before_step () {
    if (modevent.beforeStep != NULL)
        modevent.beforeStep ();
}

void sys_mod_after_step () {
    if (modevent.afterStep != NULL)
        modevent.afterStep ();
}

void sys_mod_on_pozi_smile (int smid, int lvl) {
    if (modevent.onPoziSmile != NULL)
        modevent.onPoziSmile (smid, lvl);
}

void sys_mod_on_nega_smile (int smid, int lvl) {
    if (modevent.onNegaSmile != NULL) 
        modevent.onNegaSmile (smid, lvl);
}

void sys_mod_on_fleg_smile (int smid, int lvl) {
    if (modevent.onFlegSmile != NULL)
        modevent.onFlegSmile (smid, lvl);
}

void sys_mod_on_iron_smile (int smid, int lvl) {
    if (modevent.onIronSmile != NULL)
        modevent.onIronSmile (smid, lvl);
}

void sys_mod_on_ham_smile (int smid, int lvl) {
    if (modevent.onHamSmile != NULL)
        modevent.onHamSmile (smid, lvl);
}

void sys_mod_on_killed (int plid, int murder) {
    if (modevent.onKilled != NULL) 
        modevent.onKilled (plid, murder);
}

void sys_mod_on_kill (int plid,int victim) {
    if (modevent.onKill != NULL)
        modevent.onKill (plid, victim);
}

void sys_mod_on_wall (int plid) {
    if (modevent.onWall != NULL)
        modevent.onWall (plid);
}

void sys_mod_on_self_death (int plid) {
    if (modevent.onSelfDeath != NULL)
        modevent.onSelfDeath (plid);
}

void sys_mod_on_cleared (int plid) {
    if (modevent.onCleared != NULL)
        modevent.onCleared (plid);
}

void sys_mod_on_pl_timer (int plid) {
    if (modevent.onPlTimer != NULL)
        modevent.onPlTimer (plid);
}

const char *sys_get_profile () {
    return profile;
}

const char *sys_get_images_dir () {
    return images_dir;
}

const char *sys_get_fonts_dir () {
    return fonts_dir;
}

const char *sys_get_fonts_dir_home () {
    return fonts_dir_home;
}

const char *sys_get_sounds_dir () {
    return sounds_dir;
}

const char *sys_get_sounds_dir_home () {
    return sounds_dir_home;
}

const char *sys_get_music_dir () {
    return music_dir;
}

const char *sys_get_music_dir_home () {
    return music_dir_home;
}