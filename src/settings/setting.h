#ifndef __SETTING_H__
#define __SETTING_H__

#include <string>

using namespace std;

#include "setting_defs.h"

struct Setting {
private:
    static SetSection* directory;

    static bool changed;

    static SetSection * select_section (const string & name);

    static void add_entry (SetSection *sec, const string& key, const string & value);

    static SetSection * parse_line (SetSection *sec, const string & line);

    static SetSection *find_section (const string& name);

    static SetEntry *find_entry (const SetSection *sec, const string& key);
    
public:
    static void load ();

    static void save ();

    static void print_directory ();

    static void free_directory ();

    static int read_int (const string& section, const string& key, int def);

    static void write_int (const string& section, const string& key, int value);

    static const string & read_string (const string& section, const string& key, const string & def);

    static void write_string (const string& section, const string& key, const string & value);

    static void delete_section (const string & section);
};


#endif // __SETTING_H__
