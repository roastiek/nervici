#include <SDL_keysym.h>
#include <vector>
#include <iostream>

#include "basic_defs.h"
#include "utils.h"
#include "settings/setting.h"
#include "settings/settings.h"
#include "settings/pl_info.h"

#include "settings/pl_infos.h"

using namespace Glib;
using namespace std;

#define DEFAULT_AI_COUNT 11

static PlInfo def_ais[DEFAULT_AI_COUNT] = {PlInfo (0x6600ff, "Bunnie", 0,
        "mucha", 1), PlInfo (0x00cc00, "Feeze", 0, "mucha", 1), PlInfo (
        0x66ffff, "Dyzzi", 0, "mucha", 1), PlInfo (0x6666ff, "Mexx", 0,
        "mucha", 1), PlInfo (0x999900, "Mr. Magor", 0, "mucha", 1), PlInfo (
        0x99ccff, "Der Křeček", 0, "mucha", 1), PlInfo (0x0066ff,
        "Butterflyek", 0, "mucha", 1), PlInfo (0xff3366, "cw. Orange", 0,
        "mucha", 1), PlInfo (0xccffff, "Lemonie", 0, "mucha", 1), PlInfo (
        0x66ff00, "Rabbyte", 0, "mucha", 1), PlInfo (0x0099ff, "Mortzsche", 0,
        "mucha", 1)};

vector<PlInfo*> PlInfos::players;

vector<PlInfo*> PlInfos::ais;

PlInfos::PlInfos () {
}

void PlInfos::load_players () {
    Setting& set = settings.players ();

    vector<ustring> sections = set.get_sections ();
    //players.resize (sections.size ());

    for (size_t pi = 0; pi < sections.size (); pi++) {
        const ustring& section = sections[pi];

        PlInfo* info = new PlInfo ();
        info->type = PT_Human;
        info->color = set.read_hex (section, "color", 0xffffff);
        info->name = set.read_string (section, "name", "no name");
        info->profil = set.read_string (section, "profil", "");
        info->keys.left = set.read_int (section, "left", SDLK_LEFT);
        info->keys.right = set.read_int (section, "right", SDLK_RIGHT);
        info->keys.jump = set.read_int (section, "jump", SDLK_UP);
        info->pitch = set.read_int (section, "pitch", 5);
        players.push_back(info);
    }
}

void PlInfos::load_ais () {
    Setting& set = settings.ais ();

    vector<ustring> sections = set.get_sections ();

    if (sections.size () == 0) {
        for (size_t pi = 0; pi < DEFAULT_AI_COUNT; pi++) {
            PlInfo* info = new PlInfo ();
            info->type = PT_AI;
            info->ai.id = def_ais[pi].ai.id;
            info->color = def_ais[pi].color;
            info->name = def_ais[pi].name;
            info->profil = def_ais[pi].profil;
            info->pitch = def_ais[pi].pitch;
            ais.push_back(info);
        }
    } else {
        for (size_t pi = 0; pi < sections.size (); pi++) {
            const ustring& section = sections[pi];
            PlInfo* info = new PlInfo ();
            info->type = PT_AI;
            info->ai.id = set.read_int (section, "id", 0);
            info->color = set.read_hex (section, "color", 0xff);
            info->name = set.read_string (section, "name", "none");
            info->profil = set.read_string (section, "profil", "");
            info->pitch = set.read_int (section, "pitch", 10);
            ais.push_back(info);
        }
    }
}

void PlInfos::load () {
    load_players ();
    load_ais ();
}

void PlInfos::save_players () {
    Setting& set = settings.players ();
    set.clear ();

    ustring section;
    for (size_t pi = 0; pi < players.size (); pi++) {
        section = "player" + to_string<size_t> (pi);
        const PlInfo* info = players[pi];
        
        set.write_hex (section, "color", info->color);
        set.write_string (section, "name", info->name);
        set.write_string (section, "profil", info->profil);
        set.write_int (section, "left", info->keys.left);
        set.write_int (section, "right", info->keys.right);
        set.write_int (section, "jump", info->keys.jump);
        set.write_int (section, "pitch", info->pitch);
        
        delete players[pi];
    }
    players.clear();
}

void PlInfos::save_ais () {
    Setting& set = settings.ais ();
    set.clear ();

    ustring section;
    for (size_t pi = 0; pi < ais.size (); pi++) {
        section = "plastik" + to_string<size_t> (pi);
        const PlInfo* info = ais[pi];

        set.write_hex (section, "color", info->color);
        set.write_string (section, "name", info->name);
        set.write_string (section, "profil", info->profil);
        set.write_int (section, "ai", info->ai.id);
        set.write_int (section, "pitch", info->pitch);
        
        delete ais[pi];
    }
    ais.clear();
}

void PlInfos::save_and_free () {
    save_players ();
    save_ais ();
}

void PlInfos::add (const PlInfo & info) {
    PlInfo *store = new PlInfo;
    *store = info;
    ((info.type == PT_Human) ? players : ais).push_back (store);
}

void PlInfos::remove (size_t index) {
    if (index < players.size ()) {
        PlInfo* store = players[index];
        players.erase (players.begin () + index);
        delete store;
    } else {
        index-= players.size();
        PlInfo* store = ais[index];
        ais.erase (ais.begin () + index);
        delete store;
    }
}
