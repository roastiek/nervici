/* 
 * File:   team.h
 * Author: bobo
 *
 * Created on 21. červenec 2010, 20:34
 */

#ifndef TEAM_H
#define	TEAM_H

#include "int_type.h"
#include "settings/team_info.h"
#include "game/pl_state.h"

class Team {
private:
    plid_tu id;
    plid_tu order;

    const TeamInfo* info;
    
    score_ti score;
    plid_tu states[PS_Count];

public:

    void initialize (plid_tu id, const TeamInfo* info);

    void uninitialize ();

    void set_score (score_ti value);

    void inc_score (score_ti delta);

    void dec_score (score_ti delta);

    void inc_state (PlState state);
    
    void dec_state (PlState state);

    void update_score ();
};

#endif	/* TEAM_H */

