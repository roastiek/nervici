#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <glibmm/fileutils.h>
#include <giomm/file.h>
#include <sstream>

#include "system.h"
#include "utils.h"

#include "settings/setting.h"

using namespace Glib;
using namespace Gio;

Setting::Setting (const Glib::ustring& place) :
filename (place) {
}

void Setting::load () {
    try {
        values.load_from_file (System::get_config_dir () + filename);
    } catch (FileError) {
    } catch (KeyFileError) {
    }
}

void Setting::save () {
    ustring old_data = "";
    try {
        KeyFile old;
        old.load_from_file (System::get_config_dir () + filename);
        old_data = old.to_data ();
    } catch (FileError) {
    } catch (KeyFileError) {
    }

    ustring new_data = values.to_data ();

    if (old_data.compare (new_data) != 0) {
        RefPtr<File> file = File::create_for_path (System::get_config_dir () + filename);
        string tag = "";
        try {
            file->replace_contents (new_data, tag, tag);
        } catch (Gio::Error) {
        }
    }
}

int Setting::read_int (const Glib::ustring& section,
        const Glib::ustring& key, int def) {
    try {
        if (values.has_key (section, key)) {
            return values.get_integer (section, key);
        }
    } catch (KeyFileError) {
    }
    write_int (section, key, def);
    return def;
}

void Setting::write_int (const Glib::ustring& section,
        const Glib::ustring& key, int value) {
    try {
        values.set_integer (section, key, value);
    } catch (KeyFileError) {
    }
}

unsigned long int Setting::read_hex (const Glib::ustring& section,
        const Glib::ustring& key, unsigned long int def) {

    try {
        if (values.has_key (section, key)) {
            ustring val = values.get_string (section, key);
            if (val != "") {
                stringstream ss (val, ios::in);
                ss >> std::hex >> def;
                return def;
            }
        }
    } catch (KeyFileError) {
    }

    write_hex (section, key, def);
    return def;
}

void Setting::write_hex (const Glib::ustring& section,
        const Glib::ustring& key, unsigned long int value) {

    stringstream ss (ios::out);
    ss << std::hex << value;
    ustring hex_value = ss.str ();

    try {
        values.set_string (section, key, hex_value);
    } catch (KeyFileError) {
    }
}

Glib::ustring Setting::read_string (const Glib::ustring& section,
        const Glib::ustring& key, const Glib::ustring & def) {
    try {
        if (values.has_key (section, key)) {
            return values.get_string (section, key);
        }
    } catch (KeyFileError) {
    }
    write_string (section, key, def);
    return def;
}

void Setting::write_string (const Glib::ustring& section,
        const Glib::ustring& key, const Glib::ustring & value) {
    try {
        values.set_string (section, key, value);
    } catch (KeyFileError) {
    }
}

vector<ustring> Setting::read_string_list (const ustring& section,
        const ustring& key, const vector<ustring>& def) {
    try {
        if (values.has_key (section, key)) {
            return values.get_string_list (section, key);
        }
    } catch (KeyFileError) {
    }
    write_string_list (section, key, def);
    return def;
}

void Setting::write_string_list (const ustring& section,
        const ustring& key, const vector<ustring>& value) {
    try {
        values.set_string_list (section, key, value);
    } catch (KeyFileError) {
    }
}

void Setting::delete_section (const Glib::ustring & section) {
    try {
        if (values.has_group (section)) {
            values.remove_group (section);
        }
    } catch (KeyFileError) {
    }
}

void Setting::delete_key (const Glib::ustring& section, const Glib::ustring key) {
    try {
        if (values.has_key (section, key)) {
            values.remove_key (section, key);
        }
    } catch (KeyFileError) {
    }
}

std::vector<Glib::ustring> Setting::get_sections () {
    try {
        return values.get_groups ();
    } catch (KeyFileError) {
        return vector<ustring > ();
    }
}

std::vector<Glib::ustring> Setting::get_keys (const Glib::ustring& section) {
    try {
        return values.get_keys (section);
    } catch (KeyFileError) {
        return vector<ustring > ();
    }
}

void Setting::clear () {
    ArrayHandle<ustring> sections = values.get_groups ();

    for (size_t si = 0; si < sections.size (); si++) {
        values.remove_group (sections.data ()[si]);
    }
}

