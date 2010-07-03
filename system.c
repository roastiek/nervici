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
#define PROFILE_FILE "/nervici.conf"
#define HOME_MODS_DIR "/mods"
#define HOME_FONTS_DIR "/fonts"
#define HOME_SOUNDS_DIR "/sounds"
#define HOME_MUSIC_DIR "/music"

typedef struct _ModEvents {
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
} ModEvents;

static char *home_dir = NULL;
static char *profile_dir = NULL;
static char *profile_file = NULL;
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

static ModEvents mod_events;
static void *mod_handle;

static char *resolv_home_dir () {
    return str_copy (getenv ("HOME"));
}

static char *resolv_profile_dir () {
    return str_concat (home_dir, PROFILE_DIR);
}

static char *resolv_profile_file () {
    return str_concat (profile_dir, PROFILE_FILE);
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

    mkdir (profile_dir, 0755);
    mkdir (mods_dir_home, 0755);
    mkdir (fonts_dir_home, 0755);
    mkdir (sounds_dir_home, 0755);
    mkdir (music_dir_home, 0755);
}

void sys_free_paths () {
    free (home_dir);
    free (profile_dir);
    free (profile_file);
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

static const ModInfo *load_mod_info (const char * filename) {
    void *handle;
    const ModInfo *result = NULL;
    
    handle = dlopen (filename, RTLD_NOW);
    if (!handle) {
        printf ("load mod  info %s\n", dlerror ());
        return NULL;
    }
    
    ModGetModInfo getMI;
    long int fake = (long int) dlsym (handle, "get_mod_info");
    getMI = (ModGetModInfo) fake;
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
    const ModInfo *info;
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
    
    mod_handle = dlopen (mod_entries.items[mid].filename, RTLD_NOW);
    if (!mod_handle) return;
    
    printf ("load mod\n");
    
    fake = (long int) dlsym (mod_handle, "after_step");
    mod_events.after_step = (ModAfterStep) fake;

    printf ("load mod %ld\n", mod_entries.count);

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
}

void sys_unload_mod () {
    dlclose (mod_handle);
}

void sys_mod_on_game_start (const GameSetting *set) {
    if (mod_events.on_game_start != NULL)
        mod_events.on_game_start (set);
}

void sys_mod_on_game_end () {
    if (mod_events.on_game_end != NULL)
        mod_events.on_game_end ();
}

void sys_mod_on_timer () {
    if (mod_events.on_timer != NULL)
        mod_events.on_timer ();
}

void sys_mod_on_death (int plid) {
    if (mod_events.on_death != NULL)
        mod_events.on_death (plid);
}

void sys_mod_before_step () {
    if (mod_events.before_step != NULL)
        mod_events.before_step ();
}

void sys_mod_after_step () {
    if (mod_events.after_step != NULL)
        mod_events.after_step ();
}

void sys_mod_on_pozi_smile (int smid, int lvl) {
    if (mod_events.on_pozi_smile != NULL)
        mod_events.on_pozi_smile (smid, lvl);
}

void sys_mod_on_nega_smile (int smid, int lvl) {
    if (mod_events.on_nega_smile != NULL)
        mod_events.on_nega_smile (smid, lvl);
}

void sys_mod_on_fleg_smile (int smid, int lvl) {
    if (mod_events.on_fleg_smile != NULL)
        mod_events.on_fleg_smile (smid, lvl);
}

void sys_mod_on_iron_smile (int smid, int lvl) {
    if (mod_events.on_iron_smile != NULL)
        mod_events.on_iron_smile (smid, lvl);
}

void sys_mod_on_ham_smile (int smid, int lvl) {
    if (mod_events.on_ham_smile != NULL)
        mod_events.on_ham_smile (smid, lvl);
}

void sys_mod_on_killed (int plid, int murder) {
    if (mod_events.on_killed != NULL)
        mod_events.on_killed (plid, murder);
}

void sys_mod_on_kill (int plid,int victim) {
    if (mod_events.on_kill != NULL)
        mod_events.on_kill (plid, victim);
}

void sys_mod_on_wall (int plid) {
    if (mod_events.on_wall != NULL)
        mod_events.on_wall (plid);
}

void sys_mod_on_self_death (int plid) {
    if (mod_events.on_self_death != NULL)
        mod_events.on_self_death (plid);
}

void sys_mod_on_cleared (int plid) {
    if (mod_events.on_cleared != NULL)
        mod_events.on_cleared (plid);
}

void sys_mod_on_pl_timer (int plid) {
    if (mod_events.on_pl_timer != NULL)
        mod_events.on_pl_timer (plid);
}

const char *sys_get_profile_file () {
    return profile_file;
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