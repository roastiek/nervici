/* 
 * File:   teams.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 17:43
 */

#ifndef TEAMS_H
#define	TEAMS_H

#include "game_info.h"
#include "pl_state.h"
#include "statistic.h"

namespace Teams {
    void initialize (const GameInfo& info);

    void uninitialize ();

    void update_score ();

    void set_score (plid_tu id, score_ti value);

    void inc_score (plid_tu id, score_ti delta);

    void dec_score (plid_tu id, score_ti delta);

    void inc_state (plid_tu id, PlState state);

    void dec_state (plid_tu id, PlState state);

    Statistic& stat (plid_tu id);
    
    void calc_stats ();
    
    void set_order (plid_tu id, plid_tu order);
    
    plid_tu get_order (plid_tu id);
    
    plid_tu get_active_count ();
    
    plid_tu get_active_id (plid_tu index);
    
    void draw_stat ();
}

#endif	/* TEAMS_H */

