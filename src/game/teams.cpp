#include <vector>

#include "logger.h"
#include "engine/render.h"
#include "settings/team_infos.h"
#include "game/team.h"

#include "game/teams.h"

using namespace std;

Teams Teams::instance;

Teams& teams = Teams::get_instance ();

Teams::Teams () {
}

bool Teams::initialize (const std::vector<const TeamInfo*>& infos) {
    logger.fineln ("initialzie teams");

    items.clear ();
    for (size_t ti = 0; ti < infos.size (); ti++) {
        const TeamInfo& tinfo = *infos[ti];
        items.push_back (new Team (ti, tinfo));
    }
    orders.resize (items.size ());

    items.push_back (new Team (orders.size (), team_infos[0]));

    if (!render.load_teams (infos))
        return false;

    return true;
}

void Teams::uninitialize () {
    logger.fineln ("freeing teams");

    render.free_teams ();

    for (size_t ti = 0; ti < items.size (); ti++) {
        delete items[ti];
    }
    items.clear ();
    orders.clear ();
}

void Teams::update_score () {
    for (size_t ti = 0; ti < orders.size (); ti++) {
        orders[ti] = 0;
    }

    for (size_t oi = 0; oi < orders.size (); oi++) {
        for (size_t ti = 0; ti < orders.size (); ti++) {
            if ((*items[oi]) > (*items[ti])) {
                orders[oi]++;
            }
        }
    }

    for (size_t ti = 0; ti < orders.size (); ti++) {
        items[ti]->order = orders[ti];
        items[ti]->update_score ();
    }
}

void Teams::calc_stats () {
    for (size_t ti = 0; ti < orders.size (); ti++) {
        items[ti]->calc_stats ();
    }
}

void Teams::draw_stat () {
    for (size_t ti = 0; ti < orders.size (); ti++) {
        items[ti]->draw_stat ();
    }
}

