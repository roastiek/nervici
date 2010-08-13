#include <glibmm/ustring.h>

#include "basic_defs.h"
#include "settings/setting.h"
#include "utils.h"

#include "settings/team_infos.h"

using namespace Glib;

namespace TeamInfos {

static TeamInfo infos[TEAMS_COUNT] = {
    {0x000000, ""},  {0xff8080, "team 17"}, {0xff80ff, "team 24"}, {0x80ff80, "team 39"}, {0x8080ff, "team 18+"}
};

const TeamInfo& get (int id) {
    return infos[id];
}

void load () {
    Setting& set = Settings::get_teams_setting ();

    ustring section;

    for (size_t ti = 1; ti < TEAMS_COUNT; ti++) {
        section = "team" + to_string<size_t> (ti);

        infos[ti].color = set.read_hex (section, "color", infos[ti].color);
        infos[ti].name = set.read_string (section, "name", infos[ti].name);
    }
}

void save () {
    Setting& set = Settings::get_teams_setting ();

    ustring section;

    for (size_t ti = 1; ti < TEAMS_COUNT; ti++) {
        section = "team" + to_string<size_t> (ti);

        set.write_hex (section, "color", infos[ti].color);
        set.write_string (section, "name", infos[ti].name);
    }
}

}
