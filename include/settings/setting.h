#ifndef __SETTING_H__
#define __SETTING_H__

#include <glibmm/ustring.h>
#include <glibmm/keyfile.h>
#include <vector>

class Setting {
private:
    Glib::KeyFile values;

    Glib::ustring filename;

public:
    Setting (const Glib::ustring& place);

    void load ();

    void save ();

    int read_int (const Glib::ustring& section,
            const Glib::ustring& key, int def);

    void write_int (const Glib::ustring& section,
            const Glib::ustring& key, int value);

    unsigned long int read_hex (const Glib::ustring& section,
            const Glib::ustring& key, unsigned long int def);

    void write_hex (const Glib::ustring& section,
            const Glib::ustring& key, unsigned long int value);

    Glib::ustring read_string (const Glib::ustring& section,
            const Glib::ustring& key, const Glib::ustring & def);

    void write_string (const Glib::ustring& section,
            const Glib::ustring& key, const Glib::ustring & value);

    std::vector<Glib::ustring> read_string_list (const Glib::ustring& section,
            const Glib::ustring& key, const std::vector<Glib::ustring>& def);

    void write_string_list (const Glib::ustring& section,
            const Glib::ustring& key, const std::vector<Glib::ustring>& value);

    void delete_section (const Glib::ustring & section);

    void delete_key (const Glib::ustring& section, const Glib::ustring key);

    std::vector<Glib::ustring> get_sections ();

    std::vector<Glib::ustring> get_keys (const Glib::ustring& section);

    void clear ();
};

namespace Settings {
    void load ();

    void save ();

    Setting& get_app_setting ();

    Setting& get_players_setting ();

    Setting& get_ais_setting ();

    Setting& get_game_setting ();

    Setting& get_teams_setting ();
}


#endif // __SETTING_H__
