#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <iostream>
#include <fstream>

#include "setting.h"
#include "system.h"

struct SetEntry {
    SetEntry *next;
    string key;
    string value;
};

struct SetSection {
    SetSection *next;
    string name;
    SetEntry *first_entry;
};

static SetSection* directory = NULL;
static bool changed;

static SetSection* select_section (const string& name) {
    SetSection *curr = directory;
    SetSection *last = NULL;
    int s;

    while (curr != NULL) {
        s = strcasecmp (curr->name.c_str (), name.c_str ());
        if (s == 0) {
            return curr;
        } else if (s > 0) {
            break;
        }

        last = curr;
        curr = curr->next;
    }

    SetSection *sec = new SetSection;
    sec->next = curr;
    last->next = sec;
    sec->name = name;
    sec->first_entry = NULL;
    changed = true;

    return sec;
}

static void add_entry (SetSection *sec, const string& key, const string& value) {
    SetEntry *curr = sec->first_entry;
    SetEntry *last = NULL;
    int s;

    while (curr != NULL) {
        s = strcasecmp (curr->key.c_str (), key.c_str ());
        if (s == 0) {
            if (strcmp (curr->value.c_str (), value.c_str ()) != 0) {
                curr->value = value;
                changed = true;
            }
            return;
        } else if (s > 0) {
            break;
        }

        last = curr;
        curr = curr->next;
    }

    if (last != NULL) {
        last->next = new SetEntry;
        last->next->next = curr;
        curr = last->next;
    } else {
        sec->first_entry = new SetEntry;
        sec->first_entry->next = curr;
        curr = sec->first_entry;
    }

    curr->key = key;
    curr->value = value;
    changed = true;
}

static SetSection *parse_line (SetSection *sec, const string& line) {
    size_t pos, len;
    string key;
    string value;
    string section_name;

    if (line[0] == '[' && line[line.length () - 1] == ']') {
        section_name = line.substr (1, line.length () - 2);
        sec = select_section (section_name);
    } else {
        pos = line.find ('=');
        if (pos != line.npos) {
            len = pos;

            while (isspace (line[len - 1])) {
                len--;
            }
            key = line.substr (0, len);

            pos++;
            while (pos <= line.length () && isspace (line[pos])) {
                pos++;
            }
            value = line.substr (pos, line.length () - pos);

            add_entry (sec, key, value);
        }
    }
    return sec;
}

void setting_load () {
    struct SetSection *sec;
    string line;
    ifstream stream;

    directory = new SetSection;
    directory->next = NULL;
    directory->name = new char[1];
    directory->name[0] = '\0';
    directory->first_entry = NULL;

    stream.open (sys_get_profile_file ().c_str (), ios::in);

    sec = directory;

    while (getline (stream, line)) {
        sec = parse_line (sec, line);
    }

    stream.close ();

    changed = false;
}

static void free_directory () {
    struct SetSection *s, *sec = directory;
    struct SetEntry *ent, *e;

    while (sec != NULL) {
        s = sec;
        sec = sec->next;

        ent = s->first_entry;
        while (ent != NULL) {
            e = ent;
            ent = ent->next;
            delete e;
        }
        delete s;
    }

    directory = NULL;
}

void setting_save () {
    SetSection *sec;
    SetEntry *ent;
    ofstream stream;


    if (changed) {
        stream.open (sys_get_profile_file ().c_str (), ios::out | ios::trunc);

        for (sec = directory; sec != NULL; sec = sec->next) {
            if (sec->name[0] == '\0') {
                continue;
            }

            stream << "[" << sec->name.c_str () << "]\n";

            ent = sec->first_entry;
            while (ent != NULL) {
                stream << ent->key.c_str () << " = " << ent->value.c_str () << "\n";
                ent = ent->next;
            }
            stream << "\n";
        }

        stream.close ();
    }
    free_directory ();
}

static struct SetSection *find_section (const string& name) {
    struct SetSection *curr = directory;
    int s;

    while (curr != NULL) {
        s = strcasecmp (curr->name.c_str (), name.c_str ());
        if (s == 0) {
            return curr;
        } else if (s > 0) {
            break;
        }
        curr = curr->next;
    }
    return NULL;
}

static struct SetEntry *find_entry (const SetSection *sec, const string& key) {
    SetEntry *curr = sec->first_entry;
    int s;

    while (curr != NULL) {
        s = strcasecmp (curr->key.c_str (), key.c_str ());
        if (s == 0) {
            return curr;
        } else if (s > 0) {
            break;
        }
        curr = curr->next;
    }
    return NULL;
}

int setting_read_int (const string& section, const string& key, int def) {
    SetSection *sec;
    SetEntry *ent;
    int value = def;

    sec = find_section (section);
    if (sec == NULL) return def;

    ent = find_entry (sec, key);
    if (ent == NULL) return def;

    value = atoi (ent->value.c_str ());

    return value;
}

void setting_write_int (const string& section, const string& key, int value) {
#define BUFF_LEN 16
    char buff[BUFF_LEN];
    SetSection *sec;

    snprintf (buff, BUFF_LEN, "%d", value);

    sec = select_section (section);
    add_entry (sec, key, buff);
}

const string& setting_read_string (const string& section, const string& key, const string& def) {
    SetSection *sec;
    SetEntry *ent;

    sec = find_section (section);
    if (sec == NULL) {
        return def;
    }

    ent = find_entry (sec, key);
    if (ent == NULL) {
        return def;
    }

    return ent->value;
}

void setting_write_string (const string& section, const string& key, const string& value) {
    SetSection *sec;

    sec = select_section (section);
    add_entry (sec, key, value);
}

/*int findSections (String prefix, String **sections) {
    struct SetSection *curr = directory;
    int count = 0;
    int s;
    int n = strlen (prefix);
    
    while (curr != NULL) {
        s = strncasecmp (curr->name, prefix, n);
        if (s == 0) {
            count++;
            (*sections) = realloc ((*sections), sizeof (char*) * count);
            (*sections)[count - 1] = curr->name;
        } else if (s > 0) {
            break;  
        }
        curr = curr->next;
    }
    return count;
}*/

void setting_delete_section (const string& section) {
    SetSection *curr = directory->next;
    SetSection *last = directory;
    SetEntry *ent, *e;

    int s;

    while (curr != NULL) {
        s = strcasecmp (curr->name.c_str (), section.c_str ());
        if (s == 0) {
            last->next = curr->next;
            ent = curr->first_entry;
            while (ent != NULL) {
                e = ent;
                ent = ent->next;
                delete e;
            }
            delete curr;
            changed = true;
            return;
        } else if (s > 0) {
            break;
        }

        last = curr;
        curr = curr->next;
    }
}

void setting_print_directory () {
    SetSection *sec = directory;
    SetEntry *ent;

    while (sec != NULL) {
        printf ("[%s]\n", sec->name.c_str ());

        ent = sec->first_entry;
        while (ent != NULL) {
            printf ("  %s = %s\n", ent->key.c_str (), ent->value.c_str ());
            ent = ent->next;
        }

        sec = sec->next;
    }
}
