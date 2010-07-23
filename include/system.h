#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <glibmm/ustring.h>
using namespace Glib;

#include "mods/game_setting.h"
#include "int_type.h"
#include "mod.h"

namespace System {

    /*
     * location of ~/.nervici/nervici.conf
     */
    const ustring & get_profile_file ();

    /*
     * location of share/nervici/images
     */
    const ustring & get_images_dir ();

    /*
     * location of share/nervici/fonts
     */
    const ustring & get_fonts_dir ();

    /*
     * location of ~/.nervici/fonts
     */
    const ustring & get_fonts_dir_home ();

    /*
     * location of share/nervici/sound
     */
    const ustring & get_sounds_dir ();

    /*
     * location of ~/.nervici/sounds
     */
    const ustring & get_sounds_dir_home ();

    /*
     * location of share/nervici/fonts
     */
    const ustring & get_music_dir ();

    /*
     * location of ~/.nervici/sounds
     */
    const ustring & get_music_dir_home ();

    /*
     * populate all system paths
     */
    void init_paths ();

    /*
     * deallocate all system paths
     */
    void free_paths ();

    /*
     * Search for mods in share/nervici/mods and !/.nervici/mods
     * Results are accesible via sys_get_mods_count and sys_get_mod
     * sys_init_paths must be call before
     */
    void find_mods ();

    /*
     * Deallocate all mods
     */
    void free_mods ();

    /*
     * Return count of avaible mods
     */
    size_t get_mods_count ();

    /*
     * Return info about mod with index mid
     */
    const Mod & get_mod (size_t mid);

    /*
     * Select and prepare mod before game start.
     * Technically it loads all event funtictions of mod
     */
    void load_mod (size_t mid, const ustring & script);

    /*
     * Disconnect from event function of mod after game ends.
     * Another mod can be loaded after.
     */
    void unload_mod ();

    /*
     * Call OnGameStart event
     * Game call it after succfully prepeared a new game and before entering game loop
     * GameSetting contains standard customizable settings, which players have set.
     */
    void mod_on_game_start (const GameSetting * set);

    /*
     * Call OnGameEvent
     * This is called after game leave game loop
     */
    void mod_on_game_end ();

    /*
     * Call OnTimer
     * This is called when game countdown is finished.
     * Timer must be started manually from mod before
     */
    void mod_on_timer ();

    void mod_on_death (plid_tu plid);

    void mod_before_step ();

    void mod_after_step ();

    void mod_on_pozi_smile (plid_tu plid, int lvl);

    void mod_on_nega_smile (plid_tu plid, int lvl);

    void mod_on_fleg_smile (plid_tu plid, int lvl);

    void mod_on_iron_smile (plid_tu plid, int lvl);

    void mod_on_ham_smile (plid_tu plid, int lvl);

    void mod_on_killed (plid_tu plid, plid_tu murder);

    void mod_on_kill (plid_tu plid, plid_tu victim);

    void mod_on_wall (plid_tu plid);

    void mod_on_self_death (plid_tu plid);

    void mod_on_cleared (plid_tu plid);

    void mod_on_pl_timer (plid_tu plid);
};


#endif // __SYSTEM_H__
