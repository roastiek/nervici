/* 
 * File:   players.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 1:02
 */

#ifndef PLAYERS_H
#define	PLAYERS_H

#include <vector>

using namespace std;

struct Players;

#include "game/player.h"

struct Players {
private:
    static vector<Player> players;
public:

    static void initialize (const GameInfo & info);

    static void uninitialize ();

    static int step ();

    static void update_score () {
        for (size_t pi = 0; pi < players.size (); pi++) {
            players[pi].update_score ();
        }
    }

    static void timer (int speed) {
        for (size_t pi = 0; pi < players.size (); pi++) {
            players[pi].timer_func (speed);
        }
    }

    static int live_pls_count () {
        int result = 0;
        for (size_t pi = 0; pi < players.size (); pi++) {
            result += players[pi].is_live ();
        }
        return result;
    }

    static void render_head () {
        for (size_t pi = 0; pi < players.size (); pi++) {
            players[pi].render_head ();
        }
    }

    static void erase () {
        for (size_t pi = 0; pi < players.size (); pi++) {
            players[pi].erase ();
        }
    }

    static int get_count () {
        return players.size ();
    }

    static void clear_pl (int plid) {
        players[plid].clear ();
    }

    static void cut_pl_at_length (int plid, int length) {
        players[plid].cut_at_length (length);
    }

    static void dec_pl_max_length (int plid, unsigned int delta) {
        players[plid].dec_max_length (delta);
    }

    static void dec_pl_score (int plid, int delta) {
        players[plid].dec_score (delta);
    }

    static void fast_clear_pl (int plid) {
        players[plid].fast_clear ();
    }

    static int get_pl_length (int plid) {
        return players[plid].get_length ();
    }

    static int get_pl_max_length (int plid) {
        return players[plid].get_max_length ();
    }

    static int get_pl_score (int plid) {
        return players[plid].get_score ();
    }

    static void give_pl_start (int plid, int start) {
        players[plid].give_start (start);
    }

    static void inc_pl_max_length (int plid, unsigned int delta) {
        players[plid].inc_max_length (delta);
    }

    static void inc_pl_score (int plid, int delta) {
        players[plid].inc_score (delta);
    }

    static int is_pl_human (int plid) {
        return players[plid].is_human ();
    }

    static int is_pl_jumping (int plid) {
        return players[plid].is_jumping ();
    }

    static int is_pl_live (int plid) {
        return players[plid].is_live ();
    }

    static void kill_pl (int plid) {
        players[plid].kill ();
    }

    static void set_pl_max_length (int plid, unsigned int length) {
        players[plid].set_max_length (length);
    }

    static void set_pl_score (int plid, int score) {
        players[plid].set_score (score);
    }

    static void set_pl_timer (int plid, int time) {
        players[plid].set_timer (time);
    }

    static void start_pl (int plid) {
        players[plid].start ();
    }

};

#endif	/* PLAYERS_H */

