/*
 * gen0.cpp
 *
 *  Created on: 24.8.2010
 *      Author: bobo
 */

#include <iostream>
#include <cmath>
#include <limits>

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
    target = random_target();
}

KeySt AIGen0::get_next_step () {
    return plan[0];
}

void AIGen0::calc (const FPoint& pos, int angle, int jumptime, plsize_tu head) {
    if (target_distance (pos) < 10) {
        target = random_target();
    }
    //cout << "plan\n";
    Render::draw_fake_face (target);
    shortes = numeric_limits<double>::max ();
    make_shortes_plan (pos, angle, jumptime, head, 0);
    //cout << shortes << '\n';
}

void AIGen0::make_shortes_plan (const FPoint& prev_pos, int prev_angle,
        int jumptime, plsize_tu head, size_t distance) {

    FPoint pos;
    int angle;
    Result res_none;
    Result res_left;
    Result res_right;
    Result res_jump;
    int next_jump;

    next_jump = (jumptime == 0) ? 0 : jumptime - 1;

    angle = prev_angle;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    res_none.target = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        clear_barier(0);
        res_none = make_plan (pos, angle, next_jump, (head + 1), distance
                + 1, KS_None, distance + 1);
    } else {
        res_none.dist = distance;
    }

    angle = (prev_angle + angles - 1) % angles;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    res_left.target = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        clear_barier(0);
        res_left = make_plan (pos, angle, next_jump, (head + 1), distance
                + 1, KS_Left, distance + 2);
    } else {
        res_left.dist = distance;
    }

    angle = (prev_angle + 1) % angles;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    res_right.target = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        clear_barier(0);
        res_right = make_plan (pos, angle, next_jump, (head + 1),
                distance + 1, KS_Right, distance + 2);
    } else {
        res_right.dist = distance;
    }

    if (jumptime == 0) {
        jumptime = JUMP_REPEAT;
        next_jump = jumptime - 1;
        angle = prev_angle;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        res_jump.target = target_distance (pos);
        if (will_survive (pos, jumptime, head)) {
            clear_barier(0);
            res_jump = make_plan (pos, angle, next_jump, (head + 1),
                    distance + 1, KS_None, distance + 1);
        } else {
            res_jump.dist = distance;
        }
        res_jump.target *= 1.1;
    } else {
        res_jump = res_none;
    }

    //cout << res_left.dist << " " << res_none.dist << " " << res_right.dist << " " << res_jump.dist << '\n';
    if (res_none >= res_left && res_none >= res_right && res_none >= res_jump) {
        plan[distance] = KS_None;
    } else if (res_left >= res_none && res_left >= res_right && res_left
            >= res_jump) {
        plan[distance] = KS_Left;
    } else if (res_right >= res_none && res_right >= res_left && res_right
            >= res_jump) {
        plan[distance] = KS_Right;
    } else {
        plan[distance] = KS_Jump;
    }
}

Result AIGen0::make_short_plan (const FPoint& prev_pos, int prev_angle,
        int jumptime, plsize_tu head, size_t distance, KeySt def) {

    FPoint pos;
    int angle;
    Result res_none;
    Result res_left;
    Result res_right;
    Result res_jump;
    int next_jump;
    double target_dist;

    next_jump = (jumptime == 0) ? 0 : jumptime - 1;

    if (def == KS_None) {
        angle = prev_angle;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        target_dist = target_distance (pos);
        if (will_survive (pos, jumptime, head)) {
            if (distance < 1) {
                res_none = make_short_plan (pos, angle, next_jump, (head + 1),
                        distance + 1, KS_None);
            } else {
                clear_barier (0);
                res_none = make_plan (pos, angle, next_jump, (head + 1),
                        distance + 1, KS_None, distance + 1);
            }
            res_none.target = (target_dist < res_none.target) ? target_dist
                    : res_none.target;
        } else {
            res_none.dist = distance;
            res_none.target = target_dist;
        }
    } else {
        res_none.dist = distance;
        res_none.target = numeric_limits<double>::max ();
    }

    if (def == KS_Left) {
        angle = (prev_angle + angles - 1) % angles;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        target_dist = target_distance (pos);
        if (will_survive (pos, jumptime, head)) {
            if (distance < 1) {
                res_left = make_short_plan (pos, angle, next_jump, (head + 1),
                        distance + 1, KS_Left);
            } else {
                clear_barier (0);
                res_left = make_plan (pos, angle, next_jump, (head + 1),
                        distance + 1, KS_None, distance + 1);
            }
            res_left.target = (target_dist < res_left.target) ? target_dist
                    : res_left.target;
        } else {
            res_left.dist = distance;
            res_left.target = target_dist;
        }
    } else {
        res_left.dist = distance;
        res_left.target = numeric_limits<double>::max ();
    }

    if (def == KS_Right) {
        angle = (prev_angle + 1) % angles;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        target_dist = target_distance (pos);
        if (will_survive (pos, jumptime, head)) {
            if (distance < 1) {
                res_right = make_short_plan (pos, angle, next_jump, (head + 1),
                        distance + 1, KS_Right);
            } else {
                clear_barier (0);
                res_right = make_plan (pos, angle, next_jump, (head + 1),
                        distance + 1, KS_None, distance + 1);
            }
            res_right.target = (target_dist < res_right.target) ? target_dist
                    : res_right.target;
        } else {
            res_right.dist = distance;
            res_right.target = target_dist;
        }
    } else {
        res_right.dist = distance;
        res_right.target = numeric_limits<double>::max ();
    }

    if (jumptime == 0) {
        jumptime = JUMP_REPEAT - 1;
        next_jump = jumptime - 1;
        angle = prev_angle;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        target_dist = target_distance (pos);
        if (will_survive (pos, jumptime, head)) {
            if (distance < 2) {
                res_jump = make_short_plan (pos, angle, next_jump, (head + 1),
                        distance + 1, KS_None);
            } else {
                clear_barier (0);
                res_jump = make_plan (pos, angle, next_jump, (head + 1),
                        distance + 1, KS_None, distance + 1);
            }
            res_jump.target = (target_dist < res_jump.target) ? target_dist
                    : res_jump.target;
        } else {
            res_jump.dist = distance;
            res_jump.target = target_dist;
        }
        res_jump.target *= 1.1;
    } else {
        res_jump.dist = distance;
        res_jump.target = numeric_limits<double>::max ();
    }

    if (res_none >= res_left && res_none >= res_right && res_none >= res_jump) {
        return res_none;
    } else if (res_left >= res_none && res_left >= res_right && res_left
            >= res_jump) {
        return res_left;
    } else if (res_right >= res_none && res_right >= res_left && res_right
            >= res_jump) {
        return res_right;
    } else {
        return res_jump;
    }
}

Result AIGen0::make_plan (const FPoint& prev_pos, int prev_angle, int jumptime,
        plsize_tu head, size_t distance, KeySt def, size_t max) {
    FPoint pos;
    Result res_none;
    Result res_left;
    Result res_right;
    int next_jump;
    int angle = prev_angle;
    double target_dist;

    if (distance == MAX_STEPS) {
        return Result (MAX_STEPS, numeric_limits<double>::max ());
    }

    barier[distance / 20]++;
    if (barier[distance / 20] > 40) {
        //clear_barier ((distance / 20) + 1);
        return Result (distance, numeric_limits<double>::max ());
    }

    next_jump = (jumptime > 1) ? jumptime - 1 : 0;

    if (def == KS_Left && distance <= max) {
        angle = (prev_angle + angles - 1) % angles;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        target_dist = target_distance (pos);
        if (will_survive (pos, jumptime, head)) {
            res_left = make_plan (pos, angle, next_jump, head + 1,
                    distance + 1, KS_Left, max);

            res_left.target = (target_dist < res_left.target) ? target_dist
                    : res_left.target;
            return res_left;
        }
        return Result (distance, target_dist);
    }

    if (def == KS_Right && distance <= max) {
        angle = (prev_angle + 1) % angles;
        pos.x = prev_pos.x + icos[angle] / 2;
        pos.y = prev_pos.y + isin[angle] / 2;
        target_dist = target_distance (pos);
        if (will_survive (pos, jumptime, head)) {
            res_right = make_plan (pos, angle, next_jump, head + 1, distance
                    + 1, KS_Right, max);

            res_right.target = (target_dist < res_right.target) ? target_dist
                    : res_right.target;
            return res_right;
        }
        return Result (distance, target_dist);
    }

    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    target_dist = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        res_none = make_plan (pos, angle, next_jump, head + 1, distance + 1,
                KS_None, distance + 1);

        res_none.target = (target_dist < res_none.target) ? target_dist
                : res_none.target;

        if (res_none.dist == MAX_STEPS) {
            return res_none;
        }

        size_t delta = res_none.dist - distance;
        if (delta < 4 || (delta > 36 && delta < 44) || (delta > 76 && delta
                < 80)) {
            return res_none;
        }
        if (delta > 80) {
            res_none.dist = distance + 80;
        }
    } else {
        res_none.dist = distance;
        res_none.target = target_dist;
    }

    angle = (prev_angle + angles - 1) % angles;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    target_dist = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        res_left = make_plan (pos, angle, next_jump, head + 1, distance + 1,
                KS_Left, res_none.dist);

        res_left.target = (target_dist < res_left.target) ? target_dist
                : res_left.target;

        if (res_left.dist == MAX_STEPS) {
            return res_left;
        }
    } else {
        res_left.dist = distance;
        res_left.target = target_dist;
    }

    angle = (prev_angle + 1) % angles;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    target_dist = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        res_right = make_plan (pos, angle, next_jump, head + 1, distance + 1,
                KS_Right, res_none.dist);

        res_right.target = (target_dist < res_right.target) ? target_dist
                : res_right.target;

        if (res_right.dist == MAX_STEPS) {
            return res_right;
        }
    } else {
        res_right.dist = distance;
        res_right.target = target_dist;
    }

    if (res_none >= res_left && res_none >= res_right) {
        return res_none;
    } else if (res_left >= res_none && res_left >= res_right) {
        return res_left;
    } else {
        return res_right;
    }
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

double AIGen0::target_distance (const FPoint& pos) {
    double delta_x = pos.x - target.x;
    double delta_y = pos.y - target.y;
    double res = sqrt (delta_x * delta_x + delta_y * delta_y);
    return (res > 5) ? res : 5;
}

void AIGen0::clear_barier (int from) {
    for (int bi = from; bi < 8; bi++) {
        barier[bi] = 0;
    }
}

Point AIGen0::random_target () {
    Point result;
    result.x = 10 + random () % (World::get_width () - 20);
    result.y = 10 + random () % (World::get_height () - 20);
    return result;
}
