/*
 * gen0.cpp
 *
 *  Created on: 24.8.2010
 *      Author: bobo
 */

#include <iostream>

#include "main.h"
#include "basic_defs.h"
#include "point.h"
#include "game/world.h"
#include "game/death_cause.h"
#include "engine/render.h"

#include "ais/gen0.h"

using namespace std;

Index::Index (size_t v) :
    value (v % MAX_STEPS) {

}

inline Index::operator size_t () const {
    return value;
}

inline Index& Index::operator ++ (int) {
    value++;
    value %= MAX_STEPS;
    return *this;
}

AIGen0::AIGen0 (plid_tu nid) :
    id (nid) {
    plan.resize (MAX_STEPS, KS_None);
}

KeySt AIGen0::get_next_step () {
    return plan[0];
}

void AIGen0::calc (const FPoint& pos, int angle, int jump_time, plsize_tu head) {
    //cout << "id: " << int(id) << '\n';
    KeySt store = plan[1];
    size_t len = make_plan (pos, angle, jump_time, head, 0, KS_None, 0);
    cout << int (store) << " new " << int (plan[0]) << " len: " << len <<'\n';
}

size_t AIGen0::make_plan (const FPoint& prev_pos, int prev_angle, int jumptime,
        plsize_tu head, size_t distance, KeySt def, size_t max) {
    FPoint pos;
    size_t dis_none;
    size_t dis_left;
    size_t dis_right;
    int next_jump;
    int angle = prev_angle;
    
    if (distance == MAX_STEPS) {
        return MAX_STEPS;
    }

    next_jump = (jumptime > 0) ? jumptime - 1 : 0;

    if (def == KS_Left && distance <= max) {
        angle = (prev_angle + angles - 1) % angles;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        if (will_survive (pos, jumptime, head)) {
            dis_left = make_plan (pos, angle, next_jump, (head + 1),
                    distance + 1, KS_Left, max);

            plan[distance] = KS_Left;
            return dis_left;
        }
        return distance;
    }

    if (def == KS_Right && distance <= max) {
        angle = (prev_angle + 1) % angles;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        if (will_survive (pos, jumptime, head)) {
            dis_right = make_plan (pos, angle, next_jump, (head + 1) ,
                    distance + 1, KS_Right, max);

            plan[distance] = KS_Right;
            return dis_right;
        }
        return distance;
    }

    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    if (will_survive (pos, jumptime, head)) {
        dis_none = make_plan (pos, angle, next_jump, (head + 1) ,
                distance + 1, KS_None, distance + 1);

        if (dis_none == MAX_STEPS) {
            plan[distance] = KS_None;
            return MAX_STEPS;
        }
    } else {
        dis_none = distance;
    }
    
    angle = (prev_angle + angles - 1) % angles;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    if (will_survive (pos, jumptime, head)) {
        dis_left = make_plan (pos, angle, next_jump, (head + 1) ,
                distance + 1, KS_Left, dis_none);

        if (dis_left == MAX_STEPS) {
            plan[distance] = KS_Left;
            return MAX_STEPS;
        }
    } else {
        dis_left = distance;
    }

    angle = (prev_angle + 1) % angles;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    if (will_survive (pos, jumptime, head)) {
        dis_right = make_plan (pos, angle, next_jump, (head + 1) ,
                distance + 1, KS_Right, dis_none);

        if (dis_right == MAX_STEPS) {
            plan[distance] = KS_Right;
            return MAX_STEPS;
        }
    } else {
        dis_right = distance;
    }
    
    if (distance == 0) {
        cout << distance << " : "  << dis_none << " < " << dis_left << " > " << dis_right << '\n';
    }

    if (dis_none >= dis_left && dis_none >= dis_right) {
        plan[distance] = KS_None;
    } else if (dis_left >= dis_none && dis_left >= dis_right) {
        plan[distance] = KS_Left;
    } else {
        plan[distance] = KS_Right;
    }
    return dis_none;
}

bool AIGen0::will_survive (const FPoint& pos, int jumptime, plsize_tu head) {
    Point ipos;
    DeathCause cause;

    ipos.x = pos.x - 1;
    ipos.y = pos.y - 1;
    fields.calc (pos);
    //Render::draw_fake_face(ipos);
    if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
        return World::will_survive (ipos, fields, id, head, cause);
    } else {
        return World::simple_will_survive (ipos, fields);
    }
}

