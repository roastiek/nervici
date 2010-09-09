/*
 * gen0.cpp
 *
 *  Created on: 24.8.2010
 *      Author: bobo
 */

#include <iostream>
#include <cmath>
#include <limits>
#include <glibmm/threadpool.h>
#include <glibmm/thread.h>

#include "main.h"
#include "basic_defs.h"
#include "point.h"
#include "game/world.h"
#include "game/death_cause.h"
#include "engine/render.h"
#include "game/smiles.h"
#include "game/player.h"
#include "game/smile.h"

#include "ais/gen0.h"

using namespace std;
using namespace Glib;

#define MAX_TARGET_TRIES 20

static ThreadPool* pool = NULL;

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

AIGen0::AIGen0 (Player& pl) :
    player (pl) {

    if (pool == NULL) {
        pool = new ThreadPool ();
    }

    ready = true;
    plan = KS_None;
    random_target ();
    smile_target = smiles.count ();
    target_tries = MAX_TARGET_TRIES;
}

KeySt AIGen0::get_next_step () {
    while (!ready) {
        Thread::yield ();
    }
    return plan;
}

void AIGen0::calc (const FPoint& pos, int angle, int jumptime, plsize_tu head) {
    calc_pos = pos;
    calc_angle = angle;
    calc_jumptime = jumptime;
    calc_head = head;
    ready = false;

    pool->push (sigc::mem_fun<void, AIGen0> (this, &AIGen0::work));
}

void AIGen0::make_shortes_plan (const FPoint& prev_pos, int prev_angle,
        int jumptime, plsize_tu head, size_t distance) {

    FPoint pos;
    int angle;
    Result res_none;
    Result res_left;
    Result res_right;
    Result res_jump = Result (0, 0);
    Result res;
    int next_jump;

    next_jump = (jumptime == 0) ? 0 : jumptime - 1;

    angle = prev_angle;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    res_none.target = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        clear_barier (0);
        res_none = make_plan (pos, angle, next_jump, (head + 1), distance + 1,
                KS_None, distance + 1);
    } else {
        res_none.dist = distance;
    }

    angle = (prev_angle + angles - 1) % angles;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    res_left.target = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        clear_barier (0);
        res_left = make_plan (pos, angle, next_jump, (head + 1), distance + 1,
                KS_Left, distance + 10);

        clear_barier (0);
        res = make_plan (pos, angle, next_jump, (head + 1), distance + 1,
                KS_Left, distance + 40);
        res_left = (res_left >= res) ? res_left : res;

        clear_barier (0);
        res = make_plan (pos, angle, next_jump, (head + 1), distance + 1,
                KS_Left, distance + 80);
        res_left = (res_left >= res) ? res_left : res;

    } else {
        res_left.dist = distance;
    }

    angle = (prev_angle + 1) % angles;
    pos.x = prev_pos.x + icos[angle] / 2;
    pos.y = prev_pos.y + isin[angle] / 2;
    res_right.target = target_distance (pos);
    if (will_survive (pos, jumptime, head)) {
        clear_barier (0);
        res_right = make_plan (pos, angle, next_jump, (head + 1), distance + 1,
                KS_Right, distance + 10);

        clear_barier (0);
        res = make_plan (pos, angle, next_jump, (head + 1), distance + 1,
                KS_Right, distance + 40);
        res_right = (res_right >= res) ? res_right : res;

        clear_barier (0);
        res = make_plan (pos, angle, next_jump, (head + 1), distance + 1,
                KS_Right, distance + 80);
        res_right = (res_right >= res) ? res_right : res;
    } else {
        res_right.dist = distance;
    }

    //off
    /*if (jumptime == 0) {
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
     res_jump.target *= 1.2;
     } else {
     res_jump = res_none;
     }*/

    //cout << res_left.dist << " " << res_none.dist << " " << res_right.dist << " " << res_jump.dist << '\n';
    if (res_none >= res_left && res_none >= res_right && res_none >= res_jump) {
        plan = KS_None;
    } else if (res_left >= res_none && res_left >= res_right && res_left
            >= res_jump) {
        plan = KS_Left;
    } else if (res_right >= res_none && res_right >= res_left && res_right
            >= res_jump) {
        plan = KS_Right;
    } else {
        plan = KS_Jump;
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

    /*   barier[distance / 20]++;
     if (barier[distance / 20] > 40) {
     //clear_barier ((distance / 20) + 1);
     return Result (distance, numeric_limits<double>::max ());
     }*/

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

        //if (res_none.dist == MAX_STEPS) {
        return res_none;
        //}

        size_t delta = res_none.dist - distance;
        /*        if (delta < 10 || (delta < 38 && delta > 42) || (delta < 78 && delta
         > 80)) {*/
        if (delta != 10 && delta != 40 && delta != 80) {
            return res_none;
        }
        if (delta > 80) {
            res_none.dist = distance + 80;
        }
    } else {
        res_none.dist = distance;
        res_none.target = target_dist;
        return res_none;
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

    ipos.x = pos.x - 1;
    ipos.y = pos.y - 1;
    fields.calc (pos);
    if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
        return World::good_for_ai (ipos, fields, player.id, head);
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

void AIGen0::work () {
   /* if (target_distance (calc_pos) < 10) {
        random_target ();
    }*/
    check_target();

    //cout << "plan\n";
    //Render::draw_fake_face (target);
    shortes = numeric_limits<double>::max ();
    make_shortes_plan (calc_pos, calc_angle, calc_jumptime, calc_head, 0);
    ready = true;
    //cout << shortes << '\n';
}

void AIGen0::check_target () {
    double dist;

    if (smile_target < smiles.count ()) {
        if (!smiles[smile_target].is_visible ()
                || !smiles[smile_target].is_good (player)) {
            smile_target = smiles.count ();
        }
    }

    if (smile_target == smiles.count ()) {
        smile_target = find_closest_smile ();
        if (smile_target < smiles.count ()) {
            target.x = smiles[smile_target].position ().x + 10;
            target.y = smiles[smile_target].position ().y + 10;
            best_target_dist = target_distance (player.get_position ());
            target_tries = 0;
            return;
        }
    }

    dist = target_distance (player.get_position ());
    if (dist <= best_target_dist && dist >= 10) {
        best_target_dist = dist;
        target_tries = 0;
    } else if (target_tries > MAX_TARGET_TRIES || dist < 10) {
        smile_target = find_closest_smile ();
        if (smile_target < smiles.count ()) {
            target.x = smiles[smile_target].position ().x + 10;
            target.y = smiles[smile_target].position ().y + 10;
        } else {
            random_target ();
        }
        best_target_dist = target_distance (player.get_position ());
        target_tries = 0;
    } else {
        target_tries++;
    }
}

void AIGen0::random_target () {
    target.x = 10 + random () % (World::get_width () - 20);
    target.y = 10 + random () % (World::get_height () - 20);
}

smileid_tu AIGen0::find_closest_smile () {
    smileid_tu result = smiles.count ();
    double best_dist = numeric_limits<double>::max ();
    double delta_x;
    double delta_y;
    double dist;

    for (smileid_tu si = 0; si < smiles.count (); si++) {
        const Smile& smile = smiles[si];
        if (smile.is_visible () && smile.is_good (player)) {
            delta_x = smile.position ().x + 10 - player.get_position ().x;
            delta_y = smile.position ().y + 10 - player.get_position ().y;
            dist = sqrt (delta_x * delta_x - delta_y * delta_y);
            if (dist < best_dist) {
                best_dist = dist;
                result = si;
            }
        }
    }
    return result;
}
