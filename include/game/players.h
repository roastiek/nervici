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
#include "fakes/pl_info.h"

class Players {
private:
    std::vector<Player*> players;

    std::vector<plid_tu> orders;

    static Players instance;
    
    Players ();

public:
    void initialize (const std::vector<const PlInfo*>& infos,
            const std::vector<plid_tu>& pl_teams, plsize_tu max_length);

    void uninitialize ();

    void finish_step_preparation ();
    
    plid_tu step (const uint8_t * keys);

    void update_score ();

    void timer (timer_ti speed);

    plid_tu live_pls_count ();

    void update_bodies ();

    void erase ();

    plid_tu count () const;

    const Player& operator [] (plid_tu index) const;

    Player& operator [] (plid_tu index);

    void calc_stats ();

    void draw_stat ();
    
    static Players& get_instance ();
};

extern Players& players;

inline plid_tu Players::count () const {
    return players.size ();
}

inline const Player& Players::operator [] (plid_tu index) const {
    return *players[index];
}

inline Player& Players::operator [] (plid_tu index) {
    return *players[index];
}

inline Players& Players::get_instance () {
    return instance;
}

#endif	/* PLAYERS_H */

