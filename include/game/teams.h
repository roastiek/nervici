/* 
 * File:   teams.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 17:43
 */

#ifndef TEAMS_H
#define	TEAMS_H

#include <vector>

#include "pl_state.h"
#include "fakes/team.h"
#include "fakes/game_info.h"

class Teams {
private:
    static std::vector<Team*> teams;
    
    static std::vector<plid_tu> orders;
    
    Teams ();
    
public:
    static void initialize (const GameInfo& info);

    static void uninitialize ();

    static void update_score ();

    static void calc_stats ();

    static plid_tu count ();
    
    static Team& at (plid_tu index);
    
    static void draw_stat ();
};

inline plid_tu Teams::count() {
    return orders.size();
}

inline Team& Teams::at (plid_tu index) {
    return *teams[index];
}


#endif	/* TEAMS_H */

