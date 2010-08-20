#include "engine/render.h"
#include "game/statistic.h"
#include "settings/team_info.h"

#include "game/team.h"

using namespace Glib;

void Team::initialize (plid_tu id, const TeamInfo* info) {
    this->id = id;
    this->info = info;

    order = id;

    for (PlState st = PS_Start; st < PS_Count; st++) {
        states[st] = 0;
    }
}

void Team::uninitialize () {
}

void Team::set_score (score_ti value) {
    stat().score = value;
}

void Team::inc_score (score_ti delta) {
    stat().score+= delta;
}

void Team::dec_score (score_ti delta) {
    stat().score-= delta;
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

    Render::draw_team_score (id, order, stat().score, state);
}

bool Team::operator > (const Team& other) {
    return stat().score < other.stat().score || (stat().score == other.stat().score && order > other.order);
}

bool Team::operator < (const Team& other) {
    return stat().score > other.stat().score || (stat().score == other.stat().score && order < other.order);
}

void Team::set_order (plid_tu value) {
    order = value;
}

void Team::calc_stats () {
	for (int sti = ST_pozi; sti < ST_count; sti++) {
		stat().smiles[sti][0] = stat().smiles[sti][1] + stat().smiles[sti][2] + stat().smiles[sti][3];
	}
}

void Team::draw_stat() {
	Render::draw_team_stat(id, order, info->name, info->color);
}

plid_tu Team::get_id() const {
	return id;
}

plid_tu Team::get_order() const {
	return order;
}

Statistic& Team::stat () {
    return statistic;
}

const Statistic& Team::stat () const {
    return statistic;
}

const ustring& Team::get_name() const {
    return info->name;
}
