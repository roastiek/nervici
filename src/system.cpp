#include <vector>
#include <giomm/file.h>

#include "system.h"

using namespace std;
using namespace Glib;
using namespace Gio;

namespace System {

static ustring config_dir;

static vector<ustring> data_dirs;

void init_paths () {
    config_dir = get_user_config_dir () + "/nervici/";
    RefPtr<File> dir = File::create_for_path (config_dir);
    try {
        dir->make_directory_with_parents ();
    } catch (Gio::Error) {
    }

    data_dirs.push_back (get_user_data_dir () + "/nervici/");

    const gchar * const *system_data_dirs = g_get_system_data_dirs ();
    for (size_t di = 0; system_data_dirs[di] != NULL; di++) {
        data_dirs.push_back (ustring (system_data_dirs[di]) + "/nervici/");
    }
}

void free_paths () {
}

const ustring& get_config_dir () {
    return config_dir;
}

size_t get_data_dirs_count () {
    return data_dirs.size ();
}

const ustring& get_data_dir (size_t index) {
    return data_dirs[index];
}

}

