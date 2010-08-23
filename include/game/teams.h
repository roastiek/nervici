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
#include "fakes/team_info.h"

class Teams {
private:
    std::vector<Team*> items;
    
    std::vector<plid_tu> orders;
    
    static Teams instance;
    
    Teams ();
    
public:
    void initialize (const std::vector<const TeamInfo*>& infos);

    void uninitialize ();

    void update_score ();

    void calc_stats ();

    plid_tu count () const;
    
    const Team& operator [] (plid_tu index) const;

    Team& operator [] (plid_tu index);
    
    void draw_stat ();
    
    static Teams& get_instance ();
};

extern Teams& teams;

inline plid_tu Teams::count() const {
    return orders.size();
}

inline const Team& Teams::operator [] (plid_tu index) const {
    return *items[index];
}

inline Team& Teams::operator [] (plid_tu index) {
    return *items[index];
}

inline Teams& Teams::get_instance () {
    return instance;
}

#endif	/* TEAMS_H */

