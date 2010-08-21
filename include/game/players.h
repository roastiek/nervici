/* 
 * File:   players.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 1:02
 */

#ifndef PLAYERS_H
#define	PLAYERS_H

#include <vector>

#include "int_type.h"
#include "fakes/player.h"
#include "fakes/game_info.h"

class Players {
private:
    static std::vector<Player*> players;

    static std::vector<plid_tu> orders;

    Players ();

public:
    static void initialize (const GameInfo & info);

    static void uninitialize ();

    static plid_tu step (const uint8_t * keys);

    static void update_score ();

    static void timer (timer_ti speed);

    static plid_tu live_pls_count ();

    static void update_bodies ();

    static void erase ();

    static plid_tu count ();

    static Player& at (plid_tu index);

    static void calc_stats ();

    static void draw_stat ();
};

inline plid_tu Players::count () {
    return players.size ();
}

inline Player& Players::at (plid_tu index) {
    return *players[index];
}

#endif	/* PLAYERS_H */

