#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <vector>

#include "fakes/glibmm_decl.h"

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
     * populate all system paths
     */
    void init_paths ();

    /*
     * Returns user's configuration's directory, typically ~/.config/nervici/
     */
    const Glib::ustring& get_config_dir () const;

    const Glib::ustring& get_data_dir () const;
    
    const Glib::ustring& get_user_data_dir () const;
    
    bool check_user_dir () const; 

    static System& get_instance ();
};

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
