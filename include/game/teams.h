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
    std::vector<Team*> teams;
    std::vector<plid_tu> orders;
    
    static Teams instance;
    
    Teams ();
    
public:
    void initialize (const GameInfo& info);

    void uninitialize ();

    void update_score ();

    void calc_stats ();

    plid_tu count ();
    
    Team& operator[] (plid_tu index);
    
    const Team& operator[] (plid_tu index) const;

    void draw_stat ();
    
    static Teams& get_instance ();
};

extern Teams& teams;

#endif	/* TEAMS_H */

