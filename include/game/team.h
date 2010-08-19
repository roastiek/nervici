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
#include "statistic.h"

class Team {
private:
    plid_tu id;
    plid_tu order;

    const TeamInfo* info;
    
    plid_tu states[PS_Count];

public:
    Statistic stat;

    void initialize (plid_tu id, const TeamInfo* info);

    void uninitialize ();

    void set_score (score_ti value);

    void inc_score (score_ti delta);

    void dec_score (score_ti delta);

    void inc_state (PlState state);
    
    void dec_state (PlState state);

    void update_score ();

    bool operator > (const Team& other);

    bool operator < (const Team& other);

    void set_order (plid_tu value);
    
    void calc_stats ();
    
    void draw_stat ();
    
    plid_tu get_id () const;
    
    plid_tu get_order () const;
};

#endif	/* TEAM_H */

