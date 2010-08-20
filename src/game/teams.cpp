#include <vector>

#include "engine/render.h"
#include "settings/team_infos.h"
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
    size_t ati = 0;
    
    for (size_t ti = 1; ti < TEAMS_COUNT; ti++) {
        if (info.team_active[ti]) {
            teams.push_back(new Team (ati, TeamInfos::get(ti)));
            ati++;
        }
    }
    orders.resize (teams.size ());
 
    teams.push_back(new Team (ati, TeamInfos::get(0)));
    
    Render::load_teams ();
}

void Teams::uninitialize () {
    Render::free_teams ();

    for (size_t ti = 0; ti < teams.size (); ti++) {
        delete teams[ti];
    }
    teams.resize(0);
    orders.resize(0);
}

void Teams::update_score () {
    for (size_t ti = 0; ti < orders.size (); ti++) {
        orders[ti] = 0;
    }

    for (size_t oi = 0; oi < orders.size (); oi++) {
        for (size_t ti = 0; ti < orders.size (); ti++) {
            if ((teams[oi]) > (teams[ti])) {
                orders[oi]++;
            }
        }
    }

    for (size_t ti = 0; ti < orders.size (); ti++) {
        teams[ti]->order  = orders[ti];
        teams[ti]->update_score ();
    }
}

void Teams::calc_stats () {
    for (size_t ti = 0; ti < orders.size (); ti++) {
        teams[ti]->calc_stats();
    }
}

plid_tu Teams::count () {
	return orders.size();
}

void Teams::draw_stat () {
    for (size_t ti = 0; ti < orders.size (); ti++) {
        teams[ti]->draw_stat();
    }
}

Team& Teams::operator [](plid_tu index) {
    return *teams[index];
}

const Team& Teams::operator [](plid_tu index) const {
    return *teams[index];
}

