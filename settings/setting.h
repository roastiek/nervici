#ifndef __SETTING_H__
#define __SETTING_H__

//#include "strutil.h"

void loadSettings();
void saveSettings();

void printDirectory();

int readInt(const char *section, const char* key, int def);
void writeInt(const char *section, const char *key, int value);

const char *readString(const char *section, const char *key, const char *def);
void writeString(const char * section, const char * key, const char *value);

int findSections(const char * prefix, const char ***sections);
void deleteSection(const char * section);

#endif // __SETTING_H__
