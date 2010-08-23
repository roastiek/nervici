#include <vector>
#include <iostream>

#include "engine/audio.h"
#include "engine/render.h"
#include "settings/team_info.h"
#include "game/fields.h"
#include "game/statistic.h"
#include "game/player.h"
#include "game/team.h"
#include "game/teams.h"

#include "game/players.h"

using namespace std;

Players Players::instance;

Players& players = Players::get_instance();

Players::Players () {

}

void Players::initialize (const std::vector<const PlInfo*>& infos,
        const std::vector<plid_tu>& pl_teams, plsize_tu max_length) {
    vector<uint32_t> tcolors;

    players.clear ();

    for (plid_tu pi = 0; pi < infos.size (); pi++) {
        const PlInfo& plnfo = *infos[pi];
        Team& team = teams[pl_teams[pi]];
        players.push_back (new Player (pi, team, plnfo, max_length));
        tcolors.push_back (team.info.color);
    }

    orders.resize (players.size ());

    Render::load_players (infos, tcolors);
    audio.load_players (infos);
}

void Players::uninitialize () {
    audio.free_players ();
    Render::free_players ();

    for (size_t pi = 0; pi < players.size (); pi++) {
        delete players[pi];
    }

    players.clear ();
}

plid_tu Players::step (const uint8_t * keys) {
    plid_tu result = 0;

    for (plid_tu pi = 0; pi < players.size (); pi++) {
        result += players[pi]->step (keys);
    }

    return result;
}

void Players::update_score () {
    for (plid_tu pi = 0; pi < orders.size (); pi++) {
        orders[pi] = 0;
    }

    for (plid_tu oi = 0; oi < players.size (); oi++) {
        for (plid_tu pi = 0; pi < players.size (); pi++) {
            if (*players[oi] > *players[pi]) {
                orders[oi]++;
            }
        }
    }

    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi]->order = orders[pi];
        players[pi]->update_score ();
    }
}

void Players::timer (timer_ti speed) {
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi]->timer_func (speed);
    }
}

plid_tu Players::live_pls_count () {
    plid_tu result = 0;
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        result += players[pi]->is_live ();
    }
    return result;
}

void Players::update_bodies () {
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi]->update_body ();
    }
}

void Players::erase () {
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi]->erase ();
    }
}

void Players::calc_stats () {
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi]->calc_stats ();
    }
}

void Players::draw_stat () {
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi]->draw_stat ();
    }
}
