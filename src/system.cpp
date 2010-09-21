#include <vector>
#include <giomm/file.h>
#include <glibmm/miscutils.h>

#include "system.h"

using namespace std;
using namespace Glib;
using namespace Gio;

System System::instance;

System& paths = System::get_instance ();

System::System () :
    config_dir (NULL) {

}

void System::init_paths () {
    config_dir = new ustring (get_user_config_dir () + "/nervici/");
    RefPtr<File> dir = File::create_for_path (*config_dir);
    try {
        dir->make_directory_with_parents ();
    } catch (Gio::Error) {
    }
    
    bool found;
    
    string ddir = getenv ("NERVICI_DIR", found);
    data_dir = new ustring (((found) ? ddir : get_current_dir()) + "/");

    user_data_dir = new ustring (Glib::get_user_data_dir () + "/nervici/");
    
    different_data_dirs = data_dir->compare(*user_data_dir) != 0;
}

System::~System () {
    if (config_dir != NULL) {
        delete config_dir;
        config_dir = NULL;
    }

    if (data_dir != NULL) {
        delete data_dir;
        data_dir = NULL;
    }
    
    if (user_data_dir != NULL) {
        delete user_data_dir;
        user_data_dir = NULL;
    }
}

