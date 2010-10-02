/*
 * Stores all kind of settings.
 */
#ifndef __SETTING_H__
#define __SETTING_H__

#include <glibmm/keyfile.h>
#include <vector>

/*
 * Setting class represents one setting file. Internally it is using KeyFile 
 * class from Glib library. It supports saving integer values in a decadic or 
 * hexadecimal form and string values. Values are addressed by a section name 
 * and a key. If key has not been found, read function returns default value 
 * and creates a new record for that key with default value. 
 */
class Setting {
private:
    Glib::KeyFile values;

    Glib::ustring filename;

public:
    /*
     * Creates a new Setting object, which will be bind with a specifed file, 
     * but does not load it yet.
     */
    Setting (const Glib::ustring& place);

    /*
     * Loads setting from file.
     */
    void load ();

    /*
     * Updates setting file, if some values has been changed from the last save.
     */
    void save ();

    int read_int (const Glib::ustring& section,
            const Glib::ustring& key,
            int def);

    void write_int (const Glib::ustring& section,
            const Glib::ustring& key,
            int value);

    unsigned long int read_hex (const Glib::ustring& section,
            const Glib::ustring& key,
            unsigned long int def);

    void write_hex (const Glib::ustring& section,
            const Glib::ustring& key,
            unsigned long int value);

    Glib::ustring read_string (const Glib::ustring& section,
            const Glib::ustring& key,
            const Glib::ustring & def);

    void write_string (const Glib::ustring& section,
            const Glib::ustring& key,
            const Glib::ustring & value);

    std::vector<Glib::ustring> read_string_list (const Glib::ustring& section,
            const Glib::ustring& key,
            const std::vector<Glib::ustring>& def);

    void write_string_list (const Glib::ustring& section,
            const Glib::ustring& key,
            const std::vector<Glib::ustring>& value);

    void delete_section (const Glib::ustring & section);

    void delete_key (const Glib::ustring& section, const Glib::ustring key);

    std::vector<Glib::ustring> get_sections ();

    std::vector<Glib::ustring> get_keys (const Glib::ustring& section);

    /*
     * Removes all keys and sections.
     */
    void clear ();
};

#endif // __SETTING_H__
