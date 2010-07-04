#ifndef __SETTING_H__
#define __SETTING_H__

void setting_load();
void setting_save();

void setting_print_directory();

int setting_read_int(const char *section, const char* key, int def);

void setting_write_int(const char *section, const char *key, int value);

/*
 * not free result
 */
const char *setting_read_string(const char *section, const char *key, const char *def);

/*
 * value is copied
 */
void setting_write_string(const char *section, const char *key, const char *value);


//int findSections(const char * prefix, const char ***sections);

void setting_delete_section(const char *section);

#endif // __SETTING_H__
