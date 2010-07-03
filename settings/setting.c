#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "setting.h"

#include "utils.h"
#include "system.h"

typedef struct SetEntry {
    struct SetEntry *next;
    char *key;
    char *value;
} SetEntry;

typedef struct SetSection {
    struct SetSection *next;
    char *name;
    SetEntry *start;
} SetSection;

static SetSection *directory = NULL;
static int changed;

static char *readOneLine (FILE *stream, char *buffer, size_t size) {
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

static SetSection *selectSection (const char *name) {
    SetSection *curr = directory;
    SetSection *last = NULL;
    int s;
    
    while (curr != NULL) {
        s = strcasecmp (curr->name, name);
        if (s == 0) {
            return curr;
        } else if (s > 0) {
            break;  
        }

        last = curr;
        curr = curr->next;
    }

    SetSection *sec = malloc (sizeof (SetSection));
    sec->next = curr;
    last->next = sec;
    sec->name = str_copy (name);
    sec->start = NULL;  
    changed = 1;

    return sec;
}

static void addEntry (SetSection *sec, const char *key, const char *value) {
    SetEntry *curr = sec->start;
    SetEntry *last = NULL;
    int s;
    
    while (curr != NULL) {
        s = strcasecmp (curr->key, key);
        if (s == 0) {
            if (strcmp (curr->value, value) != 0) {
                free (curr->value);
                curr->value = str_copy (value);
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
        last->next = malloc (sizeof (SetEntry));
        last->next->next = curr;
        curr = last->next;
    } else {
        sec->start = malloc (sizeof (SetEntry));
        sec->start->next = curr;
        curr = sec->start;
    }

    curr->key = str_copy (key);
    curr->value = str_copy (value);
    changed = 1;
}

static SetSection *parseLine (SetSection *sec, char *line) {
    size_t len;
    char *key;
    char *value;
    
    len = strlen (line);
    
    if (line[0] == '[' && line[len - 1] == ']') {
        line[len -1] = '\0';
        line = &line[1];
        sec = selectSection (line);
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
            
            addEntry (sec, key, value);
        }
    }
    return sec;
}

void loadSettings () {
    #define buffsize 1024
    char buffer[buffsize];

    struct SetSection *sec;
    FILE *stream;
    char *line;
    
    directory = malloc (sizeof (struct SetSection));
    directory->next = NULL;
    directory->name = malloc (1);
    directory->name[0] = '\0';
    directory->start = NULL;

    stream = fopen (sys_get_profile_file() , "rt");
    if (stream == NULL) return;
    
    sec = directory;
    
    line = readOneLine(stream, buffer, buffsize);
    while (line != NULL) {
        sec = parseLine (sec, line);
        line = readOneLine(stream, buffer, buffsize);
    }
    
    fclose (stream);
    
    changed = 0;
}

static void freeDirectory () {
    struct SetSection *s, *sec = directory;
    struct SetEntry *ent, *e;
    
    while (sec != NULL) {
        s = sec;
        sec = sec->next;

        ent = s->start;
        while (ent != NULL) {
            e = ent;
            ent = ent->next;
            
            free (e->key);
            free (e->value);
            free (e);
        }
        free (s->name);
        free (s);
    }
    
    directory = NULL;
}

void saveSettings () {
    struct SetSection *sec;
    struct SetEntry *ent;
    FILE *stream;

    if (changed) {
        stream = fopen (sys_get_profile_file (), "wt");
        if (stream == NULL) return;
    
    
        sec = directory;
        while (sec != NULL) {
            if (sec->name[0] != '\0') {
                fprintf (stream, "[%s]\n", sec->name);
            }
        
            ent = sec->start;
            while (ent != NULL) {
                fprintf (stream, "%s=%s\n", ent->key, ent->value);
                ent = ent->next;
            }
            fprintf (stream, "\n");
            sec = sec->next;
        }
    
        fclose (stream);
    }
    
    freeDirectory ();
}

static struct SetSection *findSection (const char *name) {
    struct SetSection *curr = directory;
    int s;
    
    while (curr != NULL) {
        s = strcasecmp (curr->name, name);
        if (s == 0) {
            return curr;
        } else if (s > 0) {
            break;  
        }
        curr = curr->next;
    }
    return NULL;
}

static struct SetEntry *findEntry (const SetSection *sec, const char *key) {
    SetEntry *curr = sec->start;
    int s;
    
    while (curr != NULL) {
        s = strcasecmp (curr->key, key);
        if (s == 0) {
            return curr;
        } else if (s > 0) {
            break;  
        }
        curr = curr->next;
    }
    return NULL;
}

int readInt (const char *section, const char *key, int def) {
    SetSection *sec;
    SetEntry *ent;
    int value = def;
    
    sec = findSection (section);
    if (sec == NULL) return def;
    
    ent = findEntry (sec, key);
    if (ent == NULL) return def;
    
    value = atoi (ent->value);
    
    return value;
}

void writeInt (const char *section, const char *key, int value) {
    #define BUFF_LEN 16
    char buff[BUFF_LEN];
    SetSection *sec;
    
    snprintf (buff, BUFF_LEN, "%d", value);
    
    sec = selectSection (section);
    addEntry (sec, key, buff);
}

const char *readString (const char *section, const char *key, const char *def) {
    SetSection *sec;
    SetEntry *ent;
    
    sec = findSection (section);
    if (sec == NULL) {
        return def;
    }
    
    ent = findEntry (sec, key);
    if (ent == NULL) {
        return def;
    }
    
    return ent->value;
}

void writeString (const char *section, const char *key, const char *value) {
    SetSection *sec;
    
    sec = selectSection (section);
    addEntry (sec, key, value);
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

void deleteSection (const char *section) {
    SetSection *curr = directory->next;
    SetSection *last = directory;
    SetEntry *ent, *e;
    
    int s;
    
    while (curr != NULL) {
        s = strcasecmp (curr->name, section);
        if (s == 0) {
            last->next = curr->next;
            free (curr->name);
            ent = curr->start;
            while (ent != NULL) {
                e = ent;
                ent = ent->next;
                free (e->key);
                free (e->value);
                free (e);
            }
            free (curr);
            changed = 1;
            return;
        } else if (s > 0) {
            break;  
        }

        last = curr;
        curr = curr->next;
    }
}

void printDirectory () {
    SetSection *sec = directory;
    SetEntry *ent;
    
    while (sec != NULL) {
        printf ("[%s]\n", sec->name);
        
        ent = sec->start;
        while (ent != NULL) {
            printf ("  %s = %s\n", ent->key, ent->value);
            
            ent = ent->next;
        }
        
        sec = sec->next;
    }
}
