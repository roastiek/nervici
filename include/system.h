/*
 * Used for determing paths to basic directories. 
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <vector>

#include "fakes/glibmm_decl.h"

/*
 * Using sigleton pattern
 * System class determinates and stores paths to basic directries.
 */
class System {
private:

    Glib::ustring* config_dir;

    Glib::ustring* data_dir;

    Glib::ustring* user_data_dir;

    bool different_data_dirs;
    
    static System instance;
    
    System ();

    ~System ();

public:
    /*
     * Determinates all systems paths.
     */
    void init_paths ();

    /*
     * Returns user's configuration directory, usually ~/.config/nervici/
     */
    const Glib::ustring& get_config_dir () const;

    /*
     * Returns system's data directory, usually /usr/share/nervici/
     */
    const Glib::ustring& get_data_dir () const;
    
    /*
     * Returns user's data directory, usually ~/.local/share/nervici/
     */
    const Glib::ustring& get_user_data_dir () const;
    
    /*
     * Returns if system's and user's data dirs are different. If they are same,
     * it can save some time by searching for game files only one time.
     */
    bool check_user_dir () const; 

    static System& get_instance ();
};

/*
 * Helper variable for accessing instance of System.
 */
extern System& paths;

inline const Glib::ustring& System::get_config_dir () const {
    return *config_dir;
}

inline const Glib::ustring& System::get_data_dir () const {
    return *data_dir;
}

inline const Glib::ustring& System::get_user_data_dir () const {
    return *user_data_dir;
}

inline bool System::check_user_dir() const {
    return different_data_dirs;
}

inline System& System::get_instance() {
    return instance;
}

#endif // __SYSTEM_H__
