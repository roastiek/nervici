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

namespace Teams {
    void initialize (const GameInfo& info);

    void uninitialize ();

    void update_score ();

    void set_score (plid_tu id, score_ti value);

    void inc_score (plid_tu id, score_ti delta);

    void dec_score (plid_tu id, score_ti delta);

    void inc_state (plid_tu id, PlState state);

    void dec_state (plid_tu id, PlState state);

}

#endif	/* TEAMS_H */

