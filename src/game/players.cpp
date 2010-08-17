#include <vector>
using namespace std;

#include "engine/audio.h"
#include "engine/render.h"
#include "settings/pl_infos.h"
#include "game/player.h"
#include "game/teams.h"

#include "game/players.h"

namespace Players {

static vector<Player> players;
static vector<plid_tu> orders;

void initialize (const GameInfo& info) {
    players.resize (info.setting.playersCount);
    orders.resize (info.setting.playersCount);

    int p = 0;
    for (size_t pi = 0; pi < 16; pi++) {
        if (info.pl_ids[pi] >= 0) {
            players[p].initialize (p, info.pls_team[pi], &PlInfos::get (info.pl_ids[pi]), info.setting.maxLength);
            p++;
        }
    }

    Render::load_players (info);
    Audio::load_players (info);
}

void uninitialize () {
    Audio::free_players ();
    Render::free_players ();

    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].uninitialize ();
    }

    players.clear ();
}

plid_tu step () {
    uint8_t *keys;
    plid_tu result = 0;

    keys = SDL_GetKeyState (NULL);

    for (plid_tu pi = 0; pi < players.size (); pi++) {
        result += players[pi].step (keys);
        //result += (players[pi].state == psLive);
    }

    return result;
}

void update_score () {
    for (plid_tu pi = 0; pi < orders.size (); pi++) {
        orders[pi] = 0;
    }


    for (plid_tu oi = 0; oi < players.size (); oi++) {
        for (plid_tu pi = 0; pi < players.size (); pi++) {
            if (players[oi] > players[pi]) {
                orders[oi]++;
            }
        }
    }

    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi].set_order (orders[pi]);
        players[pi].update_score ();
    }
}

void timer (timer_ti speed) {
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi].timer_func (speed);
    }
}

plid_tu live_pls_count () {
    plid_tu result = 0;
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        result += players[pi].is_live ();
    }
    return result;
}

void update_bodies () {
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi].update_body ();
    }
}

void erase () {
    for (plid_tu pi = 0; pi < players.size (); pi++) {
        players[pi].erase ();
    }
}

plid_tu get_count () {
    return players.size ();
}

void clear_pl (plid_tu plid) {
    players[plid].clear ();
}

void cut_pl_at_length (plid_tu plid, plsize_tu length) {
    players[plid].cut_at_length (length);
}

void dec_pl_max_length (plid_tu plid, plsize_tu delta) {
    players[plid].dec_max_length (delta);
}

void dec_pl_score (plid_tu plid, score_ti delta) {
    players[plid].dec_score (delta);
}

void fast_clear_pl (plid_tu plid) {
    players[plid].fast_clear ();
}

uint_fast16_t get_pl_length (plid_tu plid) {
    return players[plid].get_length ();
}

uint_fast16_t get_pl_max_length (plid_tu plid) {
    return players[plid].get_max_length ();
}

int_fast32_t get_pl_score (plid_tu plid) {
    return players[plid].get_score ();
}

void give_pl_start (plid_tu plid, startid_tu start) {
    players[plid].give_start (start);
}

void inc_pl_max_length (plid_tu plid, plsize_tu delta) {
    players[plid].inc_max_length (delta);
}

void inc_pl_score (plid_tu plid, score_ti delta) {
    players[plid].inc_score (delta);
}

bool is_pl_human (plid_tu plid) {
    return players[plid].is_human ();
}

bool is_pl_jumping (plid_tu plid) {
    return players[plid].is_jumping ();
}

bool is_pl_live (plid_tu plid) {
    return players[plid].is_live ();
}

void kill_pl (plid_tu plid) {
    players[plid].kill ();
}

void set_pl_max_length (plid_tu plid, plsize_tu length) {
    players[plid].set_max_length (length);
}

void set_pl_score (plid_tu plid, score_ti score) {
    players[plid].set_score (score);
}

void set_pl_timer (plid_tu plid, timer_ti time) {
    players[plid].set_timer (time);
}

void start_pl (plid_tu plid) {
    players[plid].start ();
}

void revive_pl (plid_tu plid) {
    players[plid].revive ();
}


void set_pl_ironize (plid_tu plid, score_ti value) {
    players[plid].set_ironize (value);
}

void inc_pl_ironize (plid_tu plid, score_ti delta) {
    players[plid].inc_ironize (delta);
}

void dec_pl_ironize (plid_tu plid, score_ti delta) {
    players[plid].dec_ironize (delta);
}

score_ti get_pl_ironze (plid_tu plid) {
    return players[plid].get_ironize ();
}

IPlayer& get_player (plid_tu plid) {
    return players[plid];
}

Statistic& stat(plid_tu plid) {
    return players[plid].stat;
}

Statistic& team_stat(plid_tu plid) {
    return Teams::stat(players[plid].get_team ());
}

}