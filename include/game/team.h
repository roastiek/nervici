/* 
 * File:   team.h
 * Author: bobo
 *
 * Created on 21. červenec 2010, 20:34
 */

#ifndef TEAM_H
#define	TEAM_H

#include "int_type.h"
#include "pl_state.h"
#include "mods/iteam.h"
#include "game/statistic.h"
#include "fakes/team_info.h"

class Team : public ITeam {
private:

    plid_tu states[PS_Count];
    
public:
    const plid_tu id;

    const TeamInfo& info;

    plid_tu order;

    Statistic stat;

    Team (plid_tu id, const TeamInfo& info);

    ~Team ();

    void inc_state (PlState state);

    void dec_state (PlState state);

    void update_score ();

    bool operator > (const Team& other) const;

    bool operator < (const Team& other) const;

    void calc_stats ();

    void draw_stat ();
    
    const Glib::ustring& get_name () const;
    
    plid_tu get_id () const;
};

#endif	/* TEAM_H */

