#ifndef __SETTING_H__
#define __SETTING_H__

#include <glibmm/ustring.h>
using namespace Glib;

namespace Setting {
    void load ();

    void save ();

    void print_directory ();

    void free_directory ();

    long int read_int (const ustring& section, const ustring& key, int def);

    void write_int (const ustring& section, const ustring& key, long int value);

    const ustring & read_string (const ustring& section, const ustring& key, const ustring & def);

    void write_string (const ustring& section, const ustring& key, const ustring & value);

    void delete_section (const ustring & section);
}


#endif // __SETTING_H__
