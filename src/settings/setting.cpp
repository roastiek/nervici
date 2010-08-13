#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <glibmm-2.4/glibmm/fileutils.h>
#include <giomm/file.h>
#include <sstream>

#include "system.h"
#include "utils.h"

#include "settings/setting.h"

using namespace Glib;
using namespace Gio;

Setting::Setting (const Glib::ustring& place):
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
            def = values.get_integer (section, key);
        }
    } catch (KeyFileError) {
    }
    return def;
}

void Setting::write_int (const Glib::ustring& section,
        const Glib::ustring& key, int value) {
    try {
        if (values.has_key (section, key)) {
            if (values.get_integer (section, key) == value) return;
        }
    } catch (KeyFileError) {
    }

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
            }
        }
    } catch (KeyFileError) {
    }

    return def;
}

void Setting::write_hex (const Glib::ustring& section,
        const Glib::ustring& key, unsigned long int value) {

    stringstream ss (ios::out);
    ss << std::hex << value;
    ustring hex_value = ss.str ();

    try {
        if (values.has_key (section, key)) {
            if (values.get_string (section, key).compare (hex_value) == 0) return;
        }
    } catch (KeyFileError) {
    }

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
    return def;
}

void Setting::write_string (const Glib::ustring& section,
        const Glib::ustring& key, const Glib::ustring & value) {
    try {
        if (values.has_key (section, key)) {
            if (values.get_string (section, key).compare (value) == 0) return;
        }
    } catch (KeyFileError) {
    }

    try {
        values.set_string (section, key, value);
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

    for (size_t si = 0; si < sections.size (); si ++) {
        values.remove_group (sections.data ()[si]);
    }
}

namespace Settings {

static Setting app_setting ("nervici.conf");
static Setting players_setting ("players.conf");
static Setting ais_setting ("plastiks.conf");
static Setting game_setting ("game.conf");

Setting& get_app_setting () {
    return app_setting;
}

Setting& get_players_setting () {
    return players_setting;
}

Setting& get_ais_setting () {
    return ais_setting;
}

Setting& get_game_setting () {
    return game_setting;
}

void load () {
    app_setting.load ();
    players_setting.load ();
    ais_setting.load ();
    game_setting.load ();
}

void save () {
    app_setting.save ();
    players_setting.save ();
    ais_setting.save ();
    game_setting.save ();
}

}
