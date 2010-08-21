#include <vector>
#include <iostream>

#include "engine/audio.h"
#include "engine/render.h"
#include "settings/pl_infos.h"
#include "settings/team_info.h"
#include "game/game_info.h"
#include "game/fields.h"
#include "game/statistic.h"
#include "game/player.h"
#include "game/team.h"
#include "game/teams.h"

#include "game/players.h"

using namespace std;

vector<Player*> Players::players;

vector<plid_tu> Players::orders;

Players::Players () {

}

void Players::initialize (const GameInfo& info) {
    vector<const PlInfo*> infos;
    vector<const TeamInfo*> tinfos;

    Team* team_map[TEAMS_COUNT] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL};

    for (plid_tu ti = 0; ti <= Teams::count (); ti++) {
        cout << int(ti) << '\n';
        team_map[Teams::at (ti).info.id] = &Teams::at (ti);
    }

    players.clear ();

    plid_tu p = 0;
    for (plid_tu pi = 0; pi < 16; pi++) {
        int inf = info.pl_ids[pi];
        if (inf >= 0) {
            const PlInfo& plnfo = pl_infos[inf];
            Team& team = *team_map[info.pls_team[pi]];
            players.push_back (new Player (p, team, plnfo,
                    info.setting.maxLength));
            infos.push_back (&plnfo);
            tinfos.push_back (&team.info);
            p++;
        }
    }

    orders.resize (players.size ());

    Render::load_players (infos, tinfos);
    Audio::load_players (infos);
}

void Players::uninitialize () {
    Audio::free_players ();
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
