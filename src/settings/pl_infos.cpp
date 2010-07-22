#include <SDL_keysym.h>
#include <vector>
using namespace std;

#include "basic_defs.h"
#include "utils.h"
#include "settings/setting.h"

#include "settings/pl_infos.h"

namespace PlInfos {

static PlInfo ais[AIS_COUNT] = {
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
static vector<PlInfo> players;

void load () {
    int count = Setting::read_int ("players", "count", 0);
    ustring section;

    players.resize (count);
    for (size_t pi = 0; pi <players.size (); pi++) {
        section = "player" + to_string<size_t>(pi);

        players[pi].type = PT_Human;
        players[pi].color = Setting::read_int (section, "color", 0xffffff);
        players[pi].name = Setting::read_string (section, "name", "no name");
        players[pi].profil = Setting::read_string (section, "profil", "");
        players[pi].keys.left = Setting::read_int (section, "left", SDLK_LEFT);
        players[pi].keys.right = Setting::read_int (section, "right", SDLK_RIGHT);
        players[pi].keys.jump = Setting::read_int (section, "jump", SDLK_UP);
        players[pi].pitch = Setting::read_int (section, "pitch", 5);
    }

    for (size_t pi = 0; pi < AIS_COUNT; pi++) {
        section = "plastik" + to_string<size_t>(pi);

        ais[pi].type = PT_AI;
        ais[pi].ai.id = Setting::read_int (section, "id", ais[pi].ai.id);
        ais[pi].color = Setting::read_int (section, "color", ais[pi].color);
        ais[pi].name = Setting::read_string (section, "name", ais[pi].name);
        ais[pi].profil = Setting::read_string (section, "profil", ais[pi].profil);
        ais[pi].pitch = Setting::read_int (section, "pitch", ais[pi].pitch);
    }
}

void save () {
    Setting::write_int ("players", "count", players.size ());
    ustring section;

    for (size_t pi = 0; pi < players.size(); pi++) {
        section = "player" + to_string<size_t>(pi);

        Setting::write_int (section, "color", players[pi].color);
        Setting::write_string (section, "name", players[pi].name);
        Setting::write_string (section, "profil", players[pi].profil);
        Setting::write_int (section, "left", players[pi].keys.left);
        Setting::write_int (section, "right", players[pi].keys.right);
        Setting::write_int (section, "jump", players[pi].keys.jump);
        Setting::write_int (section, "pitch", players[pi].pitch);
    }

    for (size_t pi = 0; pi < AIS_COUNT; pi++) {
        section = "plastik" + to_string<size_t>(pi);

        Setting::write_int (section, "color", ais[pi].color);
        Setting::write_string (section, "name", ais[pi].name);
        Setting::write_string (section, "profil", ais[pi].profil);
        Setting::write_int (section, "ai", ais[pi].ai.id);
        Setting::write_int (section, "pitch", ais[pi].pitch);
    }
}

size_t get_count () {
    return players.size () + AIS_COUNT;
}

const PlInfo& get (size_t idi) {
    return (idi < players.size()) ? players[idi] : ais[idi - players.size()];
}
}