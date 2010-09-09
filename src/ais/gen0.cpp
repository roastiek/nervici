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
    abort = true;
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
    abort = false;

    pool->push (sigc::mem_fun<void, AIGen0> (this, &AIGen0::work));
}

inline bool AIGen0::will_survive (const FPoint& pos, int jumptime,
        plsize_tu head) {
    Point ipos;

    ipos.x = pos.x - 1;
    ipos.y = pos.y - 1;
    if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
        return World::good_for_ai (ipos, player.id, head);
    } else {
        return World::simple_good_for_ai (ipos);
    }
}

inline double AIGen0::target_distance (const FPoint& pos) {
    double delta_x = pos.x - target.x;
    double delta_y = pos.y - target.y;
    double res = delta_x * delta_x + delta_y * delta_y;
    return (res > 25) ? res : 25;
}

void AIGen0::clear_barier (int from) {
    for (int bi = from; bi < 8; bi++) {
        barier[bi] = 0;
    }
}

void AIGen0::work () {
    check_target ();

    //Render::draw_fake_face (target);
    //make_shortes_plan (calc_pos, calc_angle, calc_jumptime, calc_head, 0);
    Result res_none;
    Result res_left;
    Result res_right;
    Result test;

    res_none = test_plan (player.get_position (), player.get_angle (),
            calc_jumptime, calc_head, KS_None, 0);

    res_left = test_plan (player.get_position (), player.get_angle (),
            calc_jumptime, calc_head, KS_Left, 10);

    res_right = test_plan (player.get_position (), player.get_angle (),
            calc_jumptime, calc_head, KS_Right, 10);

    test = test_plan (player.get_position (), player.get_angle (),
            calc_jumptime, calc_head, KS_Left, 40);
    res_left = (res_left >= test) ? res_left : test;

    test = test_plan (player.get_position (), player.get_angle (),
            calc_jumptime, calc_head, KS_Right, 40);
    res_right = (res_right >= test) ? res_right : test;

    test = test_plan (player.get_position (), player.get_angle (),
            calc_jumptime, calc_head, KS_Left, 80);
    res_left = (res_left >= test) ? res_left : test;

    test = test_plan (player.get_position (), player.get_angle (),
            calc_jumptime, calc_head, KS_Right, 80);
    res_right = (res_right >= test) ? res_right : test;

    if (res_none >= res_left && res_none >= res_right) {
        plan = (res_none.jump_dist != 0) ? KS_None : KS_Jump;
    } else if (res_left >= res_none && res_left >= res_right) {
        plan = (res_left.jump_dist != 0) ? KS_Left : KS_Jump;
    } else {
        plan = (res_right.jump_dist != 0) ? KS_Right : KS_Jump;
    }

    ready = true;
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
            dist = delta_x * delta_x + delta_y * delta_y;
            if (dist < best_dist) {
                best_dist = dist;
                result = si;
            }
        }
    }
    return result;
}

Result AIGen0::test_plan (const FPoint& prev_pos, int angle, int jumptime,
        plsize_tu head, KeySt def, size_t distance) {

    FPoint pos = prev_pos;
    FPoint jump_pos;
    int jump_angle;
    Result result;

    result.dist = 0;
    result.target = numeric_limits<double>::max ();
    result.jump_dist = MAX_STEPS;
    size_t di = 0;

    for (; di < 10 && !abort; di++) {
        if (jumptime > 0)
            jumptime--;

        jump_pos.x = pos.x + icos[angle] / 2;
        jump_pos.y = pos.y + icos[angle] / 2;
        jump_angle = angle;
        if (di <= distance) {
            switch (def) {
            case KS_Left:
                angle = (angle + angles - 1) % angles;
                break;
            case KS_Right:
                angle = (angle + 1) % angles;
                break;
            default:
                break;
            }
        }
        pos.x += icos[angle] / 2;
        pos.y += isin[angle] / 2;
        if (!will_survive (pos, jumptime, head)) {
            if (jumptime == 0 && di % 2 == 0) {
                jumptime = JUMP_REPEAT;
                if (!will_survive (jump_pos, jumptime, head)) {
                    return result;
                }
                pos = jump_pos;
                angle = jump_angle;
                result.jump_dist = di;
            } else {
                return result;
            }
        }
        result.min_target (target_distance (pos));
        if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
            result.dist++;
        }
        head++;
    }

    for (; di < distance && !abort; di++) {
        if (jumptime > 0)
            jumptime--;

        switch (def) {
        case KS_Left:
            angle = (angle + angles - 1) % angles;
            break;
        case KS_Right:
            angle = (angle + 1) % angles;
            break;
        default:
            break;
        }
        pos.x += icos[angle] / 2;
        pos.y += isin[angle] / 2;
        if (!will_survive (pos, jumptime, head)) {
            return result;
        }
        result.min_target (target_distance (pos));
        if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
            result.dist++;
        }
        head++;
    }

    for (; di < MAX_STEPS && !abort; di++) {
        if (jumptime > 0)
            jumptime--;

        pos.x += icos[angle] / 2;
        pos.y += isin[angle] / 2;
        if (!will_survive (pos, jumptime, head)) {
            return result;
        }
        result.min_target (target_distance (pos));
        if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
            result.dist++;
        }
        head++;
    }

    return result;
}
