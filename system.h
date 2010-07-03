#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "mods/mods.h"

typedef struct _ModEntry {
    char *filename;
    const ModInfo *info;
} ModEntry;

const char *sys_get_profile_file ();
const char *sys_get_images_dir ();
const char *sys_get_fonts_dir ();
const char *sys_get_fonts_dir_home ();
const char *sys_get_sounds_dir ();
const char *sys_get_sounds_dir_home ();
const char *sys_get_music_dir ();
const char *sys_get_music_dir_home ();

void sys_init_paths ();
void sys_free_paths ();

void sys_find_mods ();
void sys_free_mods ();

size_t sys_get_mods_count ();
const ModEntry *sys_get_mod (size_t mid);

void sys_load_mod (size_t mid);
void sys_unload_mod ();

void sys_mod_on_game_start (const GameSetting *set);
void sys_mod_on_game_end ();
void sys_mod_on_timer ();
void sys_mod_on_death (int plid);
void sys_mod_before_step ();
void sys_mod_after_step ();
void sys_mod_on_pozi_smile (int smid, int lvl);
void sys_mod_on_nega_smile (int smid, int lvl);
void sys_mod_on_fleg_smile (int smid, int lvl);
void sys_mod_on_iron_smile (int smid, int lvl);
void sys_mod_on_ham_smile (int smid, int lvl);
void sys_mod_on_killed (int plid, int murder);
void sys_mod_on_kill (int plid,int victim);
void sys_mod_on_wall (int plid);
void sys_mod_on_self_death (int plid);
void sys_mod_on_cleared (int plid);
void sys_mod_on_pl_timer (int plid);


#endif // __SYSTEM_H__
