#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <string>
#include "mods/mods.h"

using namespace std;

/*
 * location of ~/.nervici/nervici.conf
 */
const string& sys_get_profile_file ();
/*
 * location of share/nervici/images
 */
const string& sys_get_images_dir ();
/*
 * location of share/nervici/fonts
 */
const string& sys_get_fonts_dir ();
/*
 * location of ~/.nervici/fonts
 */
const string& sys_get_fonts_dir_home ();
/*
 * location of share/nervici/sound
 */
const string& sys_get_sounds_dir ();
/*
 * location of ~/.nervici/sounds
 */
const string& sys_get_sounds_dir_home ();
/*
 * location of share/nervici/fonts
 */
const string& sys_get_music_dir ();
/*
 * location of ~/.nervici/sounds
 */
const string& sys_get_music_dir_home ();

/*
 * populate all system paths
 */
void sys_init_paths ();
/*
 * deallocate all system paths
 */
void sys_free_paths ();
/*
 * Search for mods in share/nervici/mods and !/.nervici/mods
 * Results are accesible via sys_get_mods_count and sys_get_mod
 * sys_init_paths must be call before
 */
void sys_find_mods ();
/*
 * Deallocate all mods
 */
void sys_free_mods ();

/*
 * Return count of avaible mods
 */
size_t sys_get_mods_count ();
/*
 * Return info about mod with index mid
 */
const ModInfo *sys_get_mod (size_t mid);

/*
 * Select and prepare mod before game start.
 * Technically it loads all event funtictions of mod
 */
void sys_load_mod (size_t mid);
/*
 * Disconnect from event function of mod after game ends.
 * Another mod can be loaded after.
 */
void sys_unload_mod ();
/*
 * Call OnGameStart event
 * Game call it after succfully prepeared a new game and before entering game loop
 * GameSetting contains standard customizable settings, which players have set.
 */
void sys_mod_on_game_start (const GameSetting *set);
/*
 * Call OnGameEvent
 * This is called after game leave game loop
 */
void sys_mod_on_game_end ();
/*
 * Call OnTimer
 * This is called when game countdown is finished.
 * Timer must be started manually from mod before
 */
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
