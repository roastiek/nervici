#include <glibmm/ustring.h>

#include "basic_defs.h"
#include "settings/setting.h"
#include "settings/settings.h"
#include "utils.h"
#include "settings/team_info.h"

#include "settings/team_infos.h"

using namespace Glib;
using namespace std;

const static TeamInfo def_infos[TEAMS_COUNT] = {
    {
        0,
        0x000000,
        ""},
    {
        1,
        0xff8080,
        "team 17"},
    {
        2,
        0xff80ff,
        "team 24"},
    {
        3,
        0x80ff80,
        "team 39"},
    {
        4,
        0x8080ff,
        "team 18+"}};

vector<TeamInfo*> TeamInfos::infos;

TeamInfos::TeamInfos () {
}

void TeamInfos::load () {
    Setting& set = settings.teams ();

    ustring section;

    infos.resize (TEAMS_COUNT);

    infos[0] = new TeamInfo ();
    *infos[0] = def_infos[0];

    for (size_t ti = 1; ti < TEAMS_COUNT; ti++) {
        section = "team" + to_string<size_t> (ti);

        infos[ti] = new TeamInfo ();
        infos[ti]->id = ti;
        infos[ti]->color = set.read_hex (section, "color", def_infos[ti].color);
        infos[ti]->name = set.read_string (section, "name", def_infos[ti].name);
    }
}

void TeamInfos::save_and_free () {
    Setting& set = settings.teams ();

    ustring section;

    for (size_t ti = 1; ti < infos.size(); ti++) {
        section = "team" + to_string<size_t> (ti);

        set.write_hex (section, "color", infos[ti]->color);
        set.write_string (section, "name", infos[ti]->name);
        delete infos[ti];
    }
    
    infos.clear();
}
