/*
 * mods.h
 *
 *  Created on: 21.8.2010
 *      Author: bobo
 */

#ifndef MODS_H_
#define MODS_H_

#include <vector>

#include "fakes/mod_interface.h"
#include "fakes/mod.h"
#include "fakes/mod_runner.h"
#include "fakes/glibmm_decl.h"

class Mods {
private:
    std::vector<ModRunner*> mod_runners;

    std::vector<Mod*> mods;

    Glib::Module* mod_module;

    ModInterface* mod;

    static Mods instance;
    
    void scan_mods_dir (const Glib::ustring& path, std::vector<
            Glib::ustring>& files);

    void find_mod_runners (const std::vector<Glib::ustring>& files);

    void find_scripts (const std::vector<Glib::ustring>& files);

    Mods ();
    
    ~Mods ();

public:

    /*
     * Search for mods in share/nervici/mods and !/.nervici/mods
     * Results are accesible via sys_get_mods_count and sys_get_mod
     * sys_init_paths must be call before
     */
    void find_mods ();

    /*
     * Return count of avaible mods
     */
    size_t count () const;

    /*
     * Return info about mod with index mid
     */
    const Mod& operator[] (size_t mid) const;

    /*
     * Select and prepare mod before game start.
     * Technically it loads all event funtictions of mod
     */
    void load_mod (size_t mid);

    /*
     * Disconnect from event function of mod after game ends.
     * Another mod can be loaded after.
     */
    void unload_mod ();
    
    ModInterface& face ();
    
    static Mods& get_instance ();
};

extern Mods& mods;

inline size_t Mods::count () const {
    return mods.size ();
}

inline const Mod& Mods::operator [] (size_t mid) const {
    return *mods[mid];
}

inline ModInterface& Mods::face () {
    return *mod;
}

inline Mods& Mods::get_instance() {
    return instance;
}

#endif /* MODS_H_ */
