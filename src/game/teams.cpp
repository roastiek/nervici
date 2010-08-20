#include <vector>

#include "engine/render.h"
#include "settings/team_infos.h"
#include "game/statistic.h"
#include "game/team.h"

#include "game/teams.h"

using namespace std;

Teams& teams = Teams::get_instance ();

Teams Teams::instance;

Teams& Teams::get_instance() {
    return instance;
}

Teams::Teams () {
}

void Teams::initialize (const GameInfo& info) {
    teams.resize (info.setting.teams_count + 1);

    teams[0].initialize(0, &TeamInfos::get (0));
    
    int ati = 1;
    for (int ti = 1; ti < TEAMS_COUNT; ti++) {
        if (info.team_active[ti]) {
            teams[ati].initialize (ti, &TeamInfos::get (ti));
            ati++;
        }
    }
    orders.resize (teams.size ());

    Render::load_teams (info);
}

void Teams::uninitialize () {
    Render::free_teams ();

    for (size_t ti = 0; ti < teams.size (); ti++) {
        teams[ti].uninitialize ();
    }
}

void Teams::update_score () {
    for (size_t ti = 1; ti < orders.size (); ti++) {
        orders[ti] = 0;
    }

    for (size_t oi = 1; oi < teams.size (); oi++) {
        for (size_t ti = 1; ti < teams.size (); ti++) {
            if ((teams[oi]) > (teams[ti])) {
                orders[oi]++;
            }
        }
    }

    for (size_t ti = 1; ti < teams.size (); ti++) {
        teams[ti].set_order (orders[ti]);
        teams[ti].update_score ();
    }
}

void Teams::calc_stats () {
    for (size_t ti = 1; ti < teams.size (); ti++) {
        teams[ti].calc_stats();
    }
}

int Teams::count () {
	return teams.size() - 1;
}

void Teams::draw_stat () {
    for (size_t ti = 1; ti < teams.size (); ti++) {
        teams[ti].draw_stat();
    }
}

Team& Teams::operator [](int index) {
    return teams[index + 1];
}

const Team& Teams::operator [](int index) const {
    return teams[index + 1];
}

