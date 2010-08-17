#include <vector>
using namespace std;

#include "engine/render.h"
#include "game/team.h"
#include "settings/team_infos.h"

#include "game/teams.h"

namespace Teams {

static Team teams[TEAMS_COUNT];
static vector<Team*> active;
static vector<plid_tu> orders;

void initialize (const GameInfo& info) {
    active.resize (0);

    for (int ti = 0; ti < TEAMS_COUNT; ti++) {
        teams[ti].initialize (ti, &TeamInfos::get (ti));
        if (info.team_active[ti]) {
            active.push_back (&teams[ti]);
        }
    }
    orders.resize (active.size ());

    Render::load_teams (info);
}

void uninitialize () {
    Render::free_teams ();

    for (int ti = 0; ti < TEAMS_COUNT; ti++) {
        teams[ti].uninitialize ();
    }
}

void update_score () {
    for (size_t ti = 0; ti < orders.size (); ti++) {
        orders[ti] = 0;
    }

    for (size_t oi = 0; oi < active.size (); oi++) {
        for (size_t ti = 0; ti < active.size (); ti++) {
            if ((*active[oi]) > (*active[ti])) {
                orders[oi]++;
            }
        }
    }

    for (size_t ti = 0; ti < active.size (); ti++) {
        active[ti]->set_order (orders[ti]);
        active[ti]->update_score ();
    }
}

void set_score (plid_tu id, score_ti value) {
    teams[id].set_score (value);
}

void inc_score (plid_tu id, score_ti delta) {
    teams[id].inc_score (delta);
}

void dec_score (plid_tu id, score_ti delta) {
    teams[id].dec_score (delta);
}

void inc_state (plid_tu id, PlState state) {
    teams[id].inc_state (state);
}

void dec_state (plid_tu id, PlState state) {
    teams[id].dec_state (state);
}


Statistic& stat (plid_tu id) {
    return teams[id].stat;
}

}