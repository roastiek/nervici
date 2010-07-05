#ifndef __SETTING_H__
#define __SETTING_H__

#include <string>

using namespace std;

void setting_load();

void setting_save();

void setting_print_directory();

int setting_read_int(const string& section, const string& key, int def);

void setting_write_int(const string& section, const string& key, int value);

const string& setting_read_string(const string& section, const string& key, const string& def);

void setting_write_string(const string& section, const string& key, const string& value);

//int findSections(const char * prefix, const char ***sections);

void setting_delete_section(const string& section);

#endif // __SETTING_H__
