#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <glibmm/ustring.h>

#include "fakes/mod_interface.h"
#include "fakes/mod.h"

namespace System {

/*
 * Returns user's configuration's directory, typically ~/.config/nervici/
 */
const Glib::ustring& get_config_dir ();

/*
 * Returns count of data direcotries
 */
size_t get_data_dirs_count ();

/*
 * Return one data directory, the list typically contains ~/.local/share/nervici/
 * /usr/local/share/nervici/ and /usr/share/nervici/
 */
const Glib::ustring& get_data_dir (size_t index);

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
const Mod& get_mod (size_t mid);

/*
 * Select and prepare mod before game start.
 * Technically it loads all event funtictions of mod
 */
void load_mod (size_t mid, const Glib::ustring& script);

/*
 * Disconnect from event function of mod after game ends.
 * Another mod can be loaded after.
 */
void unload_mod ();

extern ModInterface* mod;

}

#endif // __SYSTEM_H__
