#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

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
    SetEntry *start;
};

static SetSection* directory = NULL;
static int changed;

static char *read_one_line (FILE *stream, char *buffer, size_t size) {
    char *result;
    size_t len;
    
    result = fgets (buffer, size, stream);
    if (result != NULL) {
        while (isspace (result[0])) {
            result = &result[1];
        }

        len = strlen (result);
        while (isspace (result[len - 1])) {
            len--;
        }
        result[len] = '\0';
    }
    return result;
}

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
    sec->start = NULL;  
    changed = 1;

    return sec;
}

static void add_entry (SetSection *sec, const string& key, const string& value) {
    SetEntry *curr = sec->start;
    SetEntry *last = NULL;
    int s;
    
    while (curr != NULL) {
        s = strcasecmp (curr->key.c_str (), key.c_str ());
        if (s == 0) {
            if (strcmp (curr->value.c_str (), value.c_str ()) != 0) {
                curr->value = value;
                changed = 1;
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
        sec->start = new SetEntry;
        sec->start->next = curr;
        curr = sec->start;
    }

    curr->key = key;
    curr->value = value;
    changed = 1;
}

static SetSection *parse_line (SetSection *sec, char *line) {
    size_t len;
    char *key;
    char *value;
    
    len = strlen (line);
    
    if (line[0] == '[' && line[len - 1] == ']') {
        line[len -1] = '\0';
        line = &line[1];
        sec = select_section (line);
    } else {
        key = line;
        value = strchr (line, '=');
        if (value != NULL) {
            value[0] = '\0';
            value = &value[1];

            len = strlen (key);
            while (isspace (key[len - 1])) {
                len--;
            }
            key[len] = '\0';

            while (isspace (value[0]) && value[0] != '\0') {
                value = &value[1];
            }
            
            add_entry (sec, key, value);
        }
    }
    return sec;
}

void setting_load () {
    #define buffsize 1024
    char buffer[buffsize];

    struct SetSection *sec;
    FILE *stream;
    char *line;
    
    directory = (SetSection*) malloc (sizeof (struct SetSection));
    directory->next = NULL;
    directory->name = (char *) malloc (1);
    directory->name[0] = '\0';
    directory->start = NULL;

    stream = fopen (sys_get_profile_file().c_str() , "rt");
    if (stream == NULL) return;
    
    sec = directory;
    
    line = read_one_line(stream, buffer, buffsize);
    while (line != NULL) {
        sec = parse_line (sec, line);
        line = read_one_line(stream, buffer, buffsize);
    }
    
    fclose (stream);
    
    changed = 0;
}

static void free_directory () {
    struct SetSection *s, *sec = directory;
    struct SetEntry *ent, *e;
    
    while (sec != NULL) {
        s = sec;
        sec = sec->next;

        ent = s->start;
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
    struct SetSection *sec;
    struct SetEntry *ent;
    FILE *stream;

    if (changed) {
        stream = fopen (sys_get_profile_file ().c_str(), "wt");
        if (stream == NULL) return;
    
    
        sec = directory;
        while (sec != NULL) {
            if (sec->name[0] != '\0') {
                fprintf (stream, "[%s]\n", sec->name.c_str ());
            }
        
            ent = sec->start;
            while (ent != NULL) {
                fprintf (stream, "%s=%s\n", ent->key.c_str (), ent->value.c_str ());
                ent = ent->next;
            }
            fprintf (stream, "\n");
            sec = sec->next;
        }
    
        fclose (stream);
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
    SetEntry *curr = sec->start;
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
            ent = curr->start;
            while (ent != NULL) {
                e = ent;
                ent = ent->next;
                delete e;
            }
            delete curr;
            changed = 1;
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
        
        ent = sec->start;
        while (ent != NULL) {
            printf ("  %s = %s\n", ent->key.c_str (), ent->value.c_str ());
            ent = ent->next;
        }
        
        sec = sec->next;
    }
}
