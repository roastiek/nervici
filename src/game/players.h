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

    static void update_score ();

    static void timer (int speed);

    static int get_count ();

    static int get_lives_count ();

    static void erase ();

    static void clear_pl (int plid);

    static void cut_pl_at_length (int plid, int length);

    static void dec_pl_max_length (int plid, unsigned int delta);

    static void dec_pl_score (int plid, int delta);

    static void fast_clear_pl (int plid);

    static int get_pl_length (int plid);

    static int get_pl_max_length (int plid);

    static int get_pl_score (int plid);

    static void give_pl_start (int plid, int start);

    static void inc_pl_max_length (int plid, unsigned int delta);

    static void inc_pl_score (int plid, int delta);

    static int is_pl_human (int plid);

    static int is_pl_jumping (int plid);

    static int is_pl_live (int plid);

    static void kill_pl (int plid);

    static void set_pl_max_length (int plid, unsigned int length);

    static void set_pl_score (int plid, int score);

    static void set_pl_timer (int plid, int time);

    static void start_pl (int plid);

    static int live_pls_count ();

    static void render_head ();
};

#endif	/* PLAYERS_H */

