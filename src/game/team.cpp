#include "engine/render.h"
#include "settings/team_info.h"
#include "game/statistic.h"

#include "game/team.h"

using namespace Glib;

Team::Team (plid_tu nid, const TeamInfo& ninfo) :
    id (nid), info (ninfo) {

    order = nid;

    for (int st = 0; st < PS_Count; st++) {
        states[st] = 0;
    }
}

Team::~Team () {
}

void Team::inc_state (PlState state) {
    states[state]++;
}

void Team::dec_state (PlState state) {
    states[state]--;
}

void Team::update_score () {
    PlState state = PS_Erased;

    if (states[PS_Live] > 0) {
        state = PS_Live;
    } else if (states[PS_Start] > 0) {
        state = PS_Start;
    } else if (states[PS_Clear] > 0) {
        state = PS_Clear;
    } else if (states[PS_Death] > 0) {
        state = PS_Death;
    }

    render.draw_team_score (id, order, stat.score, state);
}

bool Team::operator > (const Team& other) const {
    return stat.score < other.stat.score || (stat.score == other.stat.score
            && order > other.order);
}

bool Team::operator < (const Team& other) const {
    return stat.score > other.stat.score || (stat.score == other.stat.score
            && order < other.order);
}

void Team::calc_stats () {
    for (int sti = ST_pozi; sti < ST_count; sti++) {
        stat.smiles[sti][0] = stat.smiles[sti][1] + stat.smiles[sti][2]
                + stat .smiles[sti][3];
    }
}

void Team::draw_stat () {
    render.draw_team_stat (id, order, info, stat);
}

const ustring& Team::get_name () const {
    return info.name;
}

plid_tu Team::get_id () const {
    return id;
}
