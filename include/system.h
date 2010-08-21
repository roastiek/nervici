#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "fakes/glibmm_decl.h"

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

}

#endif // __SYSTEM_H__
