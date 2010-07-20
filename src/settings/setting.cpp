//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <strings.h>
//#include <ctype.h>
#include <iostream>
#include <fstream>

#include "system.h"
#include "utils.h"

#include "setting.h"

SetSection* Setting::directory;
bool Setting::changed;

SetSection* Setting::select_section (const ustring& name) {
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

void Setting::add_entry (SetSection *sec, const ustring& key, const ustring& value) {
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

SetSection *Setting::parse_line (SetSection *sec, const ustring& line) {
    size_t pos, len;
    ustring key;
    ustring value;
    ustring section_name;

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

void Setting::load () {
    struct SetSection *sec;
    string line;
    ifstream stream;

    directory = new SetSection;
    directory->next = NULL;
    directory->name = "";
    directory->first_entry = NULL;

    stream.open (System::get_profile_file ().c_str (), ios::in);

    sec = directory;

    while (getline (stream, line)) {
        sec = parse_line (sec, line);
    }

    stream.close ();

    changed = false;
}

void Setting::free_directory () {
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

void Setting::save () {
    SetSection *sec;
    SetEntry *ent;
    ofstream stream;


    if (changed) {
        stream.open (System::get_profile_file ().c_str (), ios::out | ios::trunc);

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
}

SetSection *Setting::find_section (const ustring& name) {
    SetSection *curr = directory;
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

SetEntry *Setting::find_entry (const SetSection *sec, const ustring& key) {
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

long int Setting::read_int (const ustring& section, const ustring& key, int def) {
    SetSection *sec;
    SetEntry *ent;
    int value = def;

    sec = find_section (section);
    if (sec == NULL) return def;

    ent = find_entry (sec, key);
    if (ent == NULL) return def;

    value = atol (ent->value.c_str ());

    return value;
}

void Setting::write_int (const ustring& section, const ustring& key, long int value) {
    SetSection *sec;

    sec = select_section (section);
    add_entry (sec, key, to_string<long int>(value));
}

const ustring& Setting::read_string (const ustring& section, const ustring& key, const ustring& def) {
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

void Setting::write_string (const ustring& section, const ustring& key, const ustring& value) {
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

void Setting::delete_section (const ustring& section) {
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

void Setting::print_directory () {
    SetSection *sec = directory;
    SetEntry *ent;

    while (sec != NULL) {
        cout << '[' << sec->name.c_str () << "]\n";

        ent = sec->first_entry;
        while (ent != NULL) {
            cout << "  " << ent->key.c_str () << " = " << ent->value.c_str () << '\n';
            ent = ent->next;
        }

        sec = sec->next;
    }
}
