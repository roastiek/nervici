#include <SDL.h>

#include "engine/render.h"
#include "engine/audio.h"

#include "players.h"

vector<Player> Players::players;

void Players::initialize (const GameInfo& info) {
    players.resize (info.pl_infos.size ());

    for (size_t pi = 0; pi < info.pl_infos.size (); pi++) {
        players[pi].initialize (pi, info);
    }

    render_load_players (info);
    audio_load_players (info);
}

void Players::uninitialize () {
    audio_free_players ();
    render_free_players ();

    for (size_t pi = 0; pi < Players::players.size (); pi++) {
        Players::players[pi].uninitialize ();
    }

    Players::players.clear ();
}

void Players::erase () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].erase ();
    }
}

void Players::update_score () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].update_score ();
    }
}

void Players::timer (int speed) {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].timer_func (speed);
    }
}

int Players::step () {
    uint8_t *keys;
    int result = 0;

    keys = SDL_GetKeyState (NULL);

    for (size_t pi = 0; pi < players.size (); pi++) {
        result += players[pi].step (keys);
        //result += (players[pi].state == psLive);
    }

    return result;
}

int Players::get_lives_count () {
    int result = 0;

    for (size_t pi = 0; pi < players.size (); pi++) {
        result += players[pi].is_live ();
    }

    return result;
}

void Players::give_pl_start (int plid, int start) {
    players[plid].give_start (start);
}

void Players::start_pl (int plid) {
    players[plid].start ();
}

int Players::is_pl_live (int plid) {
    return players[plid].is_live ();
}

void Players::inc_pl_score (int plid, int delta) {
    players[plid].inc_score (delta);
}

void Players::dec_pl_score (int plid, int delta) {
    players[plid].dec_score (delta);
}

void Players::set_pl_score (int plid, int score) {
    players[plid].set_score (score);
}

int Players::get_pl_score (int plid) {
    return players[plid].get_score ();
}

int Players::is_pl_jumping (int plid) {
    return players[plid].is_jumping ();
}

int Players::is_pl_human (int plid) {
    return players[plid].is_human ();
}

void Players::kill_pl (int plid) {
    players[plid].kill ();
}

void Players::clear_pl (int plid) {
    players[plid].clear ();
}

void Players::fast_clear_pl (int plid) {
    players[plid].fast_clear ();
}

void Players::cut_pl_at_length (int plid, int length) {
    players[plid].cut_at_length (length);
}

void Players::dec_pl_max_length (int plid, unsigned int delta) {
    players[plid].dec_max_length (delta);
}

int Players::get_pl_max_length (int plid) {
    return players[plid].get_max_length ();
}

int Players::get_pl_length (int plid) {
    return players[plid].get_length ();
}

void Players::inc_pl_max_length (int plid, unsigned int delta) {
    players[plid].inc_max_length (delta);
}

void Players::set_pl_max_length (int plid, unsigned int length) {
    players[plid].set_max_length (length);
}

void Players::set_pl_timer (int plid, int time) {
    players[plid].set_timer (time);
}

int Players::live_pls_count () {
    int result = 0;
    for (size_t pi = 0; pi < players.size (); pi++) {
        result += players[pi].is_live ();
    }
    return result;
}

void Players::render_head () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].render_head ();
    }
}
