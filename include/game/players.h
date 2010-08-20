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
    static Players instance;

    std::vector<Player> players;

    std::vector<plid_tu> orders;

    Players ();
    
public:
    void initialize (const GameInfo & info);

    void uninitialize ();

    plid_tu step ();

    void update_score ();

    void timer (timer_ti speed);

    plid_tu live_pls_count ();

    void update_bodies ();

    void erase ();

    plid_tu count ();
    
    Player& operator[] (plid_tu index);
  
    const Player& operator[] (plid_tu index) const;

    void calc_stats ();
    
    void draw_stat ();
    
    static Players& get_instance ();
};

extern Players& players;

#endif	/* PLAYERS_H */

