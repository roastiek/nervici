#include "basic_defs.h"

#include "settings/team_infos.h"

namespace TeamInfos {

static TeamInfo infos[TEAMS_COUNT] = {
    {0x000000},  {0xff8080}, {0xff80ff}, {0x80ff80}, {0x8080ff}
};

const TeamInfo& get (int id) {
    return infos[id];
}

}
