#include <glibmm/ustring.h>

#include "basic_defs.h"
#include "settings/setting.h"
#include "utils.h"
#include "settings/team_info.h"

#include "settings/team_infos.h"

using namespace Glib;

const static TeamInfo def_infos[TEAMS_COUNT] = { {0, 0x000000, ""}, {1,
        0xff8080, "team 17"}, {2, 0xff80ff, "team 24"},
        {3, 0x80ff80, "team 39"}, {4, 0x8080ff, "team 18+"}};

TeamInfos TeamInfos::instance;

TeamInfos& team_infos = TeamInfos::get_instance ();

TeamInfos::TeamInfos () {
}

size_t TeamInfos::count () const {
    return infos.size ();
}

TeamInfo& TeamInfos::operator [] (size_t id) {
    return infos[id];
}

const TeamInfo& TeamInfos::operator [] (size_t id) const {
    return infos[id];
}

void TeamInfos::load () {
    Setting& set = Settings::get_teams_setting ();

    ustring section;

    infos.resize (TEAMS_COUNT);

    infos[0] = def_infos[0];

    for (size_t ti = 1; ti < TEAMS_COUNT; ti++) {
        section = "team" + to_string<size_t> (ti);

        infos[ti].id = ti;
        infos[ti].color = set.read_hex (section, "color", def_infos[ti].color);
        infos[ti].name = set.read_string (section, "name", def_infos[ti].name);
    }
}

void TeamInfos::save () {
    Setting& set = Settings::get_teams_setting ();

    ustring section;

    for (size_t ti = 1; ti < TEAMS_COUNT; ti++) {
        section = "team" + to_string<size_t> (ti);

        set.write_hex (section, "color", infos[ti].color);
        set.write_string (section, "name", infos[ti].name);
    }
}

TeamInfos& TeamInfos::get_instance () {
    return instance;
}
