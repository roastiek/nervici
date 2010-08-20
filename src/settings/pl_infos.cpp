#include <SDL_keysym.h>
#include <vector>
#include <iostream>

#include "basic_defs.h"
#include "utils.h"
#include "settings/setting.h"
#include "settings/pl_info.h"

#include "settings/pl_infos.h"

using namespace Glib;
using namespace std;

#define DEFAULT_AI_COUNT 11

static PlInfo def_ais[DEFAULT_AI_COUNT] = {
    PlInfo (0x6600ff, "Bunnie", 0, "mucha", 1),
    PlInfo (0x00cc00, "Feeze", 0, "mucha", 1),
    PlInfo (0x66ffff, "Dyzzi", 0, "mucha", 1),
    PlInfo (0x6666ff, "Mexx", 0, "mucha", 1),
    PlInfo (0x999900, "Mr. Magor", 0, "mucha", 1),
    PlInfo (0x99ccff, "Der Křeček", 0, "mucha", 1),
    PlInfo (0x0066ff, "Butterflyek", 0, "mucha", 1),
    PlInfo (0xff3366, "cw. Orange", 0, "mucha", 1),
    PlInfo (0xccffff, "Lemonie", 0, "mucha", 1),
    PlInfo (0x66ff00, "Rabbyte", 0, "mucha", 1),
    PlInfo (0x0099ff, "Mortzsche", 0, "mucha", 1)
};

PlInfos PlInfos::instance;

PlInfos& pl_infos = PlInfos::get_instance();

PlInfos::PlInfos () {
}

void PlInfos::load_players () {
    Setting& set = Settings::get_players_setting ();

    vector<ustring> sections = set.get_sections ();
    players.resize (sections.size ());

    for (size_t pi = 0; pi < players.size (); pi++) {
        const ustring& section = sections[pi];

        players[pi].type = PT_Human;
        players[pi].color = set.read_hex (section, "color", 0xffffff);
        players[pi].name = set.read_string (section, "name", "no name");
        players[pi].profil = set.read_string (section, "profil", "");
        players[pi].keys.left = set.read_int (section, "left", SDLK_LEFT);
        players[pi].keys.right = set.read_int (section, "right", SDLK_RIGHT);
        players[pi].keys.jump = set.read_int (section, "jump", SDLK_UP);
        players[pi].pitch = set.read_int (section, "pitch", 5);
    }
}

void PlInfos::load_ais () {
    Setting& set = Settings::get_ais_setting ();

    vector<ustring> sections = set.get_sections ();

    if (sections.size () == 0) {
        ais.resize (DEFAULT_AI_COUNT);
        for (size_t pi = 0; pi < DEFAULT_AI_COUNT; pi++) {

            ais[pi].type = PT_AI;
            ais[pi].ai.id = def_ais[pi].ai.id;
            ais[pi].color = def_ais[pi].color;
            ais[pi].name = def_ais[pi].name;
            ais[pi].profil = def_ais[pi].profil;
            ais[pi].pitch = def_ais[pi].pitch;
        }
    } else {

        ais.resize (sections.size ());

        for (size_t pi = 0; pi < ais.size (); pi++) {
            const ustring& section = sections[pi];

            ais[pi].type = PT_AI;
            ais[pi].ai.id = set.read_int (section, "id", 0);
            ais[pi].color = set.read_hex (section, "color", 0xff);
            ais[pi].name = set.read_string (section, "name", "none");
            ais[pi].profil = set.read_string (section, "profil", "");
            ais[pi].pitch = set.read_int (section, "pitch", 10);
        }
    }
}

void PlInfos::load () {
    load_players ();
    load_ais ();
}

void PlInfos::save_players () {
    Setting& set = Settings::get_players_setting ();
    set.clear ();

    ustring section;
    for (size_t pi = 0; pi < players.size (); pi++) {
        section = "player" + to_string<size_t > (pi);

        set.write_hex (section, "color", players[pi].color);
        set.write_string (section, "name", players[pi].name);
        set.write_string (section, "profil", players[pi].profil);
        set.write_int (section, "left", players[pi].keys.left);
        set.write_int (section, "right", players[pi].keys.right);
        set.write_int (section, "jump", players[pi].keys.jump);
        set.write_int (section, "pitch", players[pi].pitch);
    }
}

void PlInfos::save_ais () {
    Setting& set = Settings::get_ais_setting ();
    set.clear ();

    ustring section;
    for (size_t pi = 0; pi < ais.size (); pi++) {
        section = "plastik" + to_string<size_t > (pi);

        set.write_hex (section, "color", ais[pi].color);
        set.write_string (section, "name", ais[pi].name);
        set.write_string (section, "profil", ais[pi].profil);
        set.write_int (section, "ai", ais[pi].ai.id);
        set.write_int (section, "pitch", ais[pi].pitch);
    }
}

void PlInfos::save () {
    save_players ();
    save_ais ();
}

size_t PlInfos::count () {
    return players.size () + ais.size ();
}

size_t PlInfos::players_count () {
    return players.size ();
}

size_t PlInfos::ais_count () {
    return ais.size ();
}

const PlInfo & PlInfos::operator [] (size_t index) const {
    return (index < players.size ()) ? players[index] : ais[index - players.size ()];
}

PlInfo & PlInfos::operator [] (size_t index) {
    return (index < players.size ()) ? players[index] : ais[index - players.size ()];
}

void PlInfos::add (const PlInfo & info) {
    ((info.type == PT_Human) ? players : ais).push_back (info);
}

void PlInfos::remove (size_t index) {
    if (index < players.size ()) {
        players.erase (players.begin () + index);
    } else {
        ais.erase (ais.begin () + index - players.size ());
    }
}

PlInfos& PlInfos::get_instance() {
    return instance;
}
