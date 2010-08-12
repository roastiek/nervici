#ifndef __SETTING_H__
#define __SETTING_H__

#include <glibmm/ustring.h>

namespace Setting {
    void load ();

    void save ();

    void print_directory ();

    void free_directory ();

    long int read_int (const Glib::ustring& section, const Glib::ustring& key, int def);

    void write_int (const Glib::ustring& section, const Glib::ustring& key, long int value);

    const Glib::ustring & read_string (const Glib::ustring& section, const Glib::ustring& key, const Glib::ustring & def);

    void write_string (const Glib::ustring& section, const Glib::ustring& key, const Glib::ustring & value);

    void delete_section (const Glib::ustring & section);
}


#endif // __SETTING_H__
