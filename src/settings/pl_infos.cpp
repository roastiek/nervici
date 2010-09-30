#include <SDL_keysym.h>
#include <vector>

#include "basic_defs.h"
#include "utils.h"
#include "logger.h"
#include "settings/setting.h"
#include "settings/settings.h"
#include "settings/pl_info.h"

#include "settings/pl_infos.h"

using namespace Glib;
using namespace std;

#define DEFAULT_AI_COUNT 11

static PlInfo def_ais[DEFAULT_AI_COUNT] = {
    PlInfo (0x6600ff, "Bunnie", true, 160, "mucha", 1),
    PlInfo (0x00cc00, "Feeze", true, 160, "mucha", 1),
    PlInfo (0x66ffff, "Dyzzi", true, 160, "mucha", 1),
    PlInfo (0x6666ff, "Mexx", true, 160, "mucha", 1),
    PlInfo (0x999900, "Mr. Magor", true, 160, "mucha", 1),
    PlInfo (0x99ccff, "Der Křeček", true, 160, "mucha", 1),
    PlInfo (0x0066ff, "Butterflyek", true, 160, "mucha", 1),
    PlInfo (0xff3366, "cw. Orange", true, 160, "mucha", 1),
    PlInfo (0xccffff, "Lemonie", true, 160, "mucha", 1),
    PlInfo (0x66ff00, "Rabbyte", true, 160, "mucha", 1),
    PlInfo (0x0099ff, "Mortzsche", true, 160, "mucha", 1)};

PlInfos PlInfos::instance;

PlInfos& pl_infos = PlInfos::get_instance ();

PlInfos::PlInfos () {
}

PlInfos::~PlInfos () {
    logger.fineln ("freeing players setting");
    for (size_t pi = 0; pi < players.size (); pi++) {
        delete players[pi];
    }
    players.clear ();

    for (size_t ai = 0; ai < ais.size (); ai++) {
        delete ais[ai];
    }
    ais.clear ();
}

void PlInfos::load_players () {
    Setting& set = settings.players ();

    vector<ustring> sections = set.get_sections ();

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
        players.push_back (info);
    }
}

void PlInfos::load_ais () {
    Setting& set = settings.ais ();

    vector<ustring> sections = set.get_sections ();

    if (sections.size () == 0) {
        logger.fineln ("no ai found, using defaults");
        for (size_t pi = 0; pi < DEFAULT_AI_COUNT; pi++) {
            PlInfo* info = new PlInfo ();
            info->type = PT_AI;
            info->ai = def_ais[pi].ai;
            info->color = def_ais[pi].color;
            info->name = def_ais[pi].name;
            info->profil = def_ais[pi].profil;
            info->pitch = def_ais[pi].pitch;
            ais.push_back (info);
        }
    } else {
        for (size_t pi = 0; pi < sections.size (); pi++) {
            const ustring& section = sections[pi];
            PlInfo* info = new PlInfo ();
            info->type = PT_AI;
            load_ai (info->ai, section);
            info->color = set.read_hex (section, "color", 0xff);
            info->name = set.read_string (section, "name", "none");
            info->profil = set.read_string (section, "profil", "");
            info->pitch = set.read_int (section, "pitch", 10);
            ais.push_back (info);
        }
    }
}

void PlInfos::load_ai (AIInfo& info, const Glib::ustring& section) {
    Setting& set = settings.ais ();
    info.gen = set.read_int (section, "gen", 0);

    switch (info.gen) {
    case AI_GEN_0:
        info.gen0.jump = set.read_int (section, "gen0_jump", false);
        info.gen0.distance = set.read_int (section, "gen0_distance", 160);
        break;
    }
}

void PlInfos::load () {
    logger.fineln ("loading players setting");
    load_players ();
    load_ais ();
}

void PlInfos::save_players () {
    Setting& set = settings.players ();
    set.clear ();

    ustring section;
    for (size_t pi = 0; pi < players.size (); pi++) {
        section = "player";
        section += uint_to_string (pi);
        const PlInfo* info = players[pi];

        set.write_hex (section, "color", info->color);
        set.write_string (section, "name", info->name);
        set.write_string (section, "profil", info->profil);
        set.write_int (section, "left", info->keys.left);
        set.write_int (section, "right", info->keys.right);
        set.write_int (section, "jump", info->keys.jump);
        set.write_int (section, "pitch", info->pitch);
    }
}

void PlInfos::save_ais () {
    Setting& set = settings.ais ();
    set.clear ();

    ustring section;
    for (size_t pi = 0; pi < ais.size (); pi++) {
        section = "plastik";
        section += uint_to_string (pi);
        const PlInfo* info = ais[pi];

        set.write_hex (section, "color", info->color);
        set.write_string (section, "name", info->name);
        set.write_string (section, "profil", info->profil);
        save_ai (info->ai, section);
        set.write_int (section, "pitch", info->pitch);
    }
}

void PlInfos::save_ai (const AIInfo& info, const Glib::ustring& section) {
    Setting& set = settings.ais ();
    set.write_int (section, "gen", info.gen);

    switch (info.gen) {
    case AI_GEN_0:
        set.write_int (section, "gen0_jump", info.gen0.jump);
        set.write_int (section, "gen0_distance", info.gen0.distance);
        break;
    }
}

void PlInfos::save () {
    logger.fineln ("saving players setting");
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
        index -= players.size ();
        PlInfo* store = ais[index];
        ais.erase (ais.begin () + index);
        delete store;
    }
}
