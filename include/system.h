#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <vector>

#include "fakes/glibmm_decl.h"

class System {
private:

    Glib::ustring* config_dir;

    std::vector<Glib::ustring*> data_dirs;

    static System instance;
    
    System ();

    ~System ();

public:
    /*
     * populate all system paths
     */
    void init_paths ();

    /*
     * Returns user's configuration's directory, typically ~/.config/nervici/
     */
    const Glib::ustring& get_config_dir () const;

    /*
     * Returns count of data direcotries
     */
    size_t get_data_dirs_count () const ;

    /*
     * Return one data directory, the list typically contains ~/.local/share/nervici/
     * /usr/local/share/nervici/ and /usr/share/nervici/
     */
    const Glib::ustring& get_data_dir (size_t index) const;
    
    static System& get_instance ();
};

extern System& paths;

inline const Glib::ustring& System::get_config_dir () const {
    return *config_dir;
}

inline size_t System::get_data_dirs_count () const {
    return data_dirs.size ();
}

inline const Glib::ustring& System::get_data_dir (size_t index) const {
    return *data_dirs[index];
}

inline System& System::get_instance() {
    return instance;
}

#endif // __SYSTEM_H__
