
#include "game/team.h"
#include "engine/render.h"

void Team::initialize (plid_tu id, const TeamInfo* info) {
    this->id = id;
    this->info = info;

    score = 0;
    order = id;

    for (PlState st = PS_Start; st < PS_Count; st++) {
        states[st] = 0;
    }
}

void Team::uninitialize () {
}

void Team::set_score (score_ti value) {
    score = value;
}

void Team::inc_score (score_ti delta) {
    score+= delta;
}

void Team::dec_score (score_ti delta) {
    score-= delta;
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

    Render::draw_team_score (id, order, score, state);
}