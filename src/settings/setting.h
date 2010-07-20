#ifndef __SETTING_H__
#define __SETTING_H__

#include "setting_defs.h"

struct Setting {
private:
    static SetSection* directory;

    static bool changed;

    static SetSection * select_section (const ustring & name);

    static void add_entry (SetSection *sec, const ustring& key, const ustring & value);

    static SetSection * parse_line (SetSection *sec, const ustring & line);

    static SetSection *find_section (const ustring& name);

    static SetEntry *find_entry (const SetSection *sec, const ustring& key);
    
public:
    static void load ();

    static void save ();

    static void print_directory ();

    static void free_directory ();

    static long int read_int (const ustring& section, const ustring& key, int def);

    static void write_int (const ustring& section, const ustring& key, long int value);

    static const ustring & read_string (const ustring& section, const ustring& key, const ustring & def);

    static void write_string (const ustring& section, const ustring& key, const ustring & value);

    static void delete_section (const ustring & section);
};


#endif // __SETTING_H__
