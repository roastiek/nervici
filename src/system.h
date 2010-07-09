#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <string>
#include <vector>

using namespace std;

#include "mods/mods.h"

#include "system_defs.h"
#include "int_type.h"

struct System {
private:
    static string home_dir;

    static string profile_dir;

    static string profile_file;

    static string mods_dir;

    static string mods_dir_home;

    static string images_dir;

    static string fonts_dir;

    static string fonts_dir_home;

    static string sounds_dir;

    static string sounds_dir_home;

    static string music_dir;

    static string music_dir_home;

    static vector<ModRunner> mod_runners;

    static vector<Mod> mods;

    static ModEvents mod_events;

    static void *mod_handle;

    static string resolv_home_dir ();

    static string resolv_profile_dir ();

    static string resolv_profile_file ();

    static string resolv_dir (const string & dir);

    static string resolv_dir_home (const string & dir);

public:

    /*
     * location of ~/.nervici/nervici.conf
     */
    static const string & get_profile_file () {
        return profile_file;
    }

    /*
     * location of share/nervici/images
     */
    static const string & get_images_dir () {
        return images_dir;
    }

    /*
     * location of share/nervici/fonts
     */
    static const string & get_fonts_dir () {
        return fonts_dir;
    }

    /*
     * location of ~/.nervici/fonts
     */
    static const string & get_fonts_dir_home () {
        return fonts_dir_home;
    }

    /*
     * location of share/nervici/sound
     */
    static const string & get_sounds_dir () {
        return sounds_dir;
    }

    /*
     * location of ~/.nervici/sounds
     */
    static const string & get_sounds_dir_home () {
        return sounds_dir_home;
    }

    /*
     * location of share/nervici/fonts
     */
    static const string & get_music_dir () {
        return music_dir;
    }

    /*
     * location of ~/.nervici/sounds
     */
    static const string & get_music_dir_home () {
        return music_dir_home;
    }

    /*
     * populate all system paths
     */
    static void init_paths ();

    /*
     * deallocate all system paths
     */
    static void free_paths ();

    /*
     * Search for mods in share/nervici/mods and !/.nervici/mods
     * Results are accesible via sys_get_mods_count and sys_get_mod
     * sys_init_paths must be call before
     */
    static void find_mods ();

    /*
     * Deallocate all mods
     */
    static void free_mods ();

    /*
     * Return count of avaible mods
     */
    static size_t get_mods_count () {
        return mods.size ();
    }

    /*
     * Return info about mod with index mid
     */
    static const Mod& get_mod (size_t mid) {
        return mods[mid];
    }

    /*
     * Select and prepare mod before game start.
     * Technically it loads all event funtictions of mod
     */
    static void load_mod (size_t mid, const string& script);

    /*
     * Disconnect from event function of mod after game ends.
     * Another mod can be loaded after.
     */
    static void unload_mod ();

    /*
     * Call OnGameStart event
     * Game call it after succfully prepeared a new game and before entering game loop
     * GameSetting contains standard customizable settings, which players have set.
     */
    static void mod_on_game_start (const GameSetting * set) {
        if (mod_events.on_game_start != NULL)
            mod_events.on_game_start (set);
    }

    /*
     * Call OnGameEvent
     * This is called after game leave game loop
     */
    static void mod_on_game_end () {
        if (mod_events.on_game_end != NULL)
            mod_events.on_game_end ();
    }

    /*
     * Call OnTimer
     * This is called when game countdown is finished.
     * Timer must be started manually from mod before
     */
    static void mod_on_timer () {
        if (mod_events.on_timer != NULL)
            mod_events.on_timer ();
    }

    static void mod_on_death (plid_tu plid) {
        if (mod_events.on_death != NULL)
            mod_events.on_death (plid);
    }

    static void mod_before_step () {
        if (mod_events.before_step != NULL)
            mod_events.before_step ();
    }

    static void mod_after_step () {
        if (mod_events.after_step != NULL)
            mod_events.after_step ();
    }

    static void mod_on_pozi_smile (int smid, int lvl) {
        if (mod_events.on_pozi_smile != NULL)
            mod_events.on_pozi_smile (smid, lvl);
    }

    static void mod_on_nega_smile (int smid, int lvl) {
        if (mod_events.on_nega_smile != NULL)
            mod_events.on_nega_smile (smid, lvl);
    }

    static void mod_on_fleg_smile (int smid, int lvl) {
        if (mod_events.on_fleg_smile != NULL)
            mod_events.on_fleg_smile (smid, lvl);
    }

    static void mod_on_iron_smile (int smid, int lvl) {
        if (mod_events.on_iron_smile != NULL)
            mod_events.on_iron_smile (smid, lvl);
    }

    static void mod_on_ham_smile (int smid, int lvl) {
        if (mod_events.on_ham_smile != NULL)
            mod_events.on_ham_smile (smid, lvl);
    }

    static void mod_on_killed (plid_tu plid, plid_tu murder) {
        if (mod_events.on_killed != NULL)
            mod_events.on_killed (plid, murder);
    }

    static void mod_on_kill (plid_tu plid, plid_tu victim) {
        if (mod_events.on_kill != NULL)
            mod_events.on_kill (plid, victim);
    }

    static void mod_on_wall (plid_tu plid) {
        if (mod_events.on_wall != NULL)
            mod_events.on_wall (plid);
    }

    static void mod_on_self_death (plid_tu plid) {
        if (mod_events.on_self_death != NULL)
            mod_events.on_self_death (plid);
    }

    static void mod_on_cleared (plid_tu plid) {
        if (mod_events.on_cleared != NULL)
            mod_events.on_cleared (plid);
    }

    static void mod_on_pl_timer (plid_tu plid) {
        if (mod_events.on_pl_timer != NULL)
            mod_events.on_pl_timer (plid);
    }
};


#endif // __SYSTEM_H__
