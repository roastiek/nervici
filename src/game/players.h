/* 
 * File:   players.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 1:02
 */

#ifndef PLAYERS_H
#define	PLAYERS_H

#include <vector>
#include <stdint.h>

using namespace std;

struct Players;

#include "game/player.h"

struct Players {
private:
    static vector<Player> players;
public:

    static void initialize (const GameInfo & info);

    static void uninitialize ();

    static plid_tu step ();

    static void update_score () {
        for (plid_tu pi = 0; pi < players.size (); pi++) {
            players[pi].update_score ();
        }
    }

    static void timer (timer_ti speed) {
        for (plid_tu pi = 0; pi < players.size (); pi++) {
            players[pi].timer_func (speed);
        }
    }

    static plid_tu live_pls_count () {
        plid_tu result = 0;
        for (plid_tu pi = 0; pi < players.size (); pi++) {
            result += players[pi].is_live ();
        }
        return result;
    }

    static void update_bodies () {
        for (plid_tu pi = 0; pi < players.size (); pi++) {
            players[pi].update_body ();
        }
    }

    static void erase () {
        for (plid_tu pi = 0; pi < players.size (); pi++) {
            players[pi].erase ();
        }
    }

    static plid_tu get_count () {
        return players.size ();
    }

    static void clear_pl (plid_tu plid) {
        players[plid].clear ();
    }

    static void cut_pl_at_length (plid_tu plid, plsize_tu length) {
        players[plid].cut_at_length (length);
    }

    static void dec_pl_max_length (plid_tu plid, plsize_tu delta) {
        players[plid].dec_max_length (delta);
    }

    static void dec_pl_score (plid_tu plid, score_ti delta) {
        players[plid].dec_score (delta);
    }

    static void fast_clear_pl (plid_tu plid) {
        players[plid].fast_clear ();
    }

    static uint_fast16_t get_pl_length (plid_tu plid) {
        return players[plid].get_length ();
    }

    static uint_fast16_t get_pl_max_length (plid_tu plid) {
        return players[plid].get_max_length ();
    }

    static int_fast32_t get_pl_score (plid_tu plid) {
        return players[plid].get_score ();
    }

    static void give_pl_start (plid_tu plid, startid_tu start) {
        players[plid].give_start (start);
    }

    static void inc_pl_max_length (plid_tu plid, plsize_tu delta) {
        players[plid].inc_max_length (delta);
    }

    static void inc_pl_score (plid_tu plid, score_ti delta) {
        players[plid].inc_score (delta);
    }

    static bool is_pl_human (plid_tu plid) {
        return players[plid].is_human ();
    }

    static bool is_pl_jumping (plid_tu plid) {
        return players[plid].is_jumping ();
    }

    static bool is_pl_live (plid_tu plid) {
        return players[plid].is_live ();
    }

    static void kill_pl (plid_tu plid) {
        players[plid].kill ();
    }

    static void set_pl_max_length (plid_tu plid, plsize_tu length) {
        players[plid].set_max_length (length);
    }

    static void set_pl_score (plid_tu plid, score_ti score) {
        players[plid].set_score (score);
    }

    static void set_pl_timer (plid_tu plid, timer_ti time) {
        players[plid].set_timer (time);
    }

    static void start_pl (plid_tu plid) {
        players[plid].start ();
    }

};

#endif	/* PLAYERS_H */

