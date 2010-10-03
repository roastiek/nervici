/* 
 * File:   players.h
 * Author: deedrah
 *
 * Created on 8. červenec 2010, 1:02
 * Stores all players for a game.
 */

#ifndef PLAYERS_H
#define	PLAYERS_H

#include <vector>

#include "int_type.h"
#include "fakes/player.h"
#include "fakes/pl_info.h"

/*
 * Collection of all players which play the game. Most of the methods just call 
 * theirs conterparts on all players.
 */
class Players {
private:
    /*
     * List of players.
     */
    std::vector<Player*> players;

    /*
     * Used for ordering players. 
     */
    std::vector<plid_tu> orders;

    static Players instance;
    
    Players ();

public:
    /*
     * Creates players for the game.
     */
    bool initialize (const std::vector<const PlInfo*>& infos,
            const std::vector<plid_tu>& pl_teams, plsize_tu max_length);

    /*
     * Frees players after the game
     */
    void uninitialize ();

    void finish_step_preparation ();
    
    plid_tu step (const uint8_t * keys);

    void update_score ();

    void timer (timer_ti speed);

    plid_tu live_pls_count ();

    void update_bodies ();

    void erase ();

    /*
     * Returns count of players.
     */
    plid_tu count () const;

    /*
     * Accessing players by index.
     */
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

