/*
 * mods.h
 *
 *  Created on: 21.8.2010
 *      Author: bobo
 */

#ifndef MODS_H_
#define MODS_H_

#include <vector>
#include <glibmm/module.h>
#include <glibmm/ustring.h>

#include "fakes/mod_interface.h"
#include "fakes/mod.h"
#include "fakes/mod_runner.h"

class Mods {
private:
    static std::vector<ModRunner*> mod_runners;

    static std::vector<Mod*> mods;

    static Glib::Module* mod_module;

    static void scan_mods_dir (const Glib::ustring& path, std::vector<
            Glib::ustring>& files);

    static void find_mod_runners (const std::vector<Glib::ustring>& files);

    static void find_scripts (const std::vector<Glib::ustring>& files);

    Mods ();

public:
    static ModInterface* mod;

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
    static size_t count ();

    /*
     * Return info about mod with index mid
     */
    static const Mod& at (size_t mid);

    /*
     * Select and prepare mod before game start.
     * Technically it loads all event funtictions of mod
     */
    static void load_mod (size_t mid);

    /*
     * Disconnect from event function of mod after game ends.
     * Another mod can be loaded after.
     */
    static void unload_mod ();
};

inline size_t Mods::count () {
    return mods.size ();
}

inline const Mod& Mods::at (size_t mid) {
    return *mods[mid];
}

#endif /* MODS_H_ */
