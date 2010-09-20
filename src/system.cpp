#include <vector>
#include <giomm/file.h>

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

    data_dirs.push_back (new ustring (get_user_data_dir () + "/nervici/"));

    const gchar * const *system_data_dirs = g_get_system_data_dirs ();
    for (size_t di = 0; system_data_dirs[di] != NULL; di++) {
        ustring* str = new ustring (system_data_dirs[di]);
        str->append ("/nervici/");
        data_dirs.push_back (str);
    }
}

System::~System () {
    if (config_dir != NULL) {
        delete config_dir;
        config_dir = NULL;
    }

    for (size_t di = 0; di < data_dirs.size (); di++) {
        delete data_dirs[di];
    }
    data_dirs.clear ();
}

