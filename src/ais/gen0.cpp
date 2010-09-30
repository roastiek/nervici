/*
 * gen0.cpp
 *
 *  Created on: 24.8.2010
 *      Author: bobo
 */

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

#define MAX_TARGET_TRIES 40
#define MAX_STEPS 160

static ThreadPool* pool = NULL;

AIGen0::AIGen0 (Player& pl, const AIGen0Info& inf) :
    player (pl), info (inf) {

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
    //Render::draw_fake_face (target);
    return plan;
}

void AIGen0::finish () {
    abort = true;
}

void AIGen0::calc (int jumptime, plsize_tu head) {
    calc_jumptime = jumptime;
    calc_head = head;
    ready = false;
    abort = false;

    pool->push (sigc::mem_fun<void, AIGen0> (this, &AIGen0::work));
}

inline bool AIGen0::will_survive (const FPoint& pos,
        int jumptime,
        plsize_tu head) {
    Point ipos;

    ipos.x = pos.x - 1;
    ipos.y = pos.y - 1;
    if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
        return world.good_for_ai (ipos, player.id, head);
    } else {
        return world.simple_good_for_ai (ipos);
    }
}

inline double AIGen0::target_distance (const FPoint& pos) {
    double delta_x = pos.x - target.x;
    double delta_y = pos.y - target.y;
    double res = delta_x * delta_x + delta_y * delta_y;
    return (res > 25) ? res : 25;
}

void AIGen0::work () {
    check_target ();

    Result res_none;
    Result res_left;
    Result res_right;
    Result test;

    res_none = test_plan (player.get_position (),
        player.get_angle (),
        calc_jumptime,
        calc_head,
        KS_None,
        0);

    res_left = test_plan (player.get_position (),
        player.get_angle (),
        calc_jumptime,
        calc_head,
        KS_Left,
        10);

    res_right = test_plan (player.get_position (),
        player.get_angle (),
        calc_jumptime,
        calc_head,
        KS_Right,
        10);

    test = test_plan (player.get_position (),
        player.get_angle (),
        calc_jumptime,
        calc_head,
        KS_Left,
        40);
    res_left = (res_left >= test) ? res_left : test;

    test = test_plan (player.get_position (),
        player.get_angle (),
        calc_jumptime,
        calc_head,
        KS_Right,
        40);
    res_right = (res_right >= test) ? res_right : test;

    test = test_plan (player.get_position (),
        player.get_angle (),
        calc_jumptime,
        calc_head,
        KS_Left,
        80);
    res_left = (res_left >= test) ? res_left : test;

    test = test_plan (player.get_position (),
        player.get_angle (),
        calc_jumptime,
        calc_head,
        KS_Right,
        80);
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
    if (dist <= best_target_dist && dist >= 100) {
        best_target_dist = dist;
        target_tries = 0;
    } else if (target_tries > MAX_TARGET_TRIES || dist < 100) {
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
    target.x = 10 + random () % (world.get_width () - 20);
    target.y = 10 + random () % (world.get_height () - 20);
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

AIGen0::Result AIGen0::test_plan (const FPoint& prev_pos,
        int angle,
        int jumptime,
        plsize_tu head,
        KeySt def,
        size_t distance) {

    FPoint pos = prev_pos;
    Result result;

    result.dist = 0;
    result.target = numeric_limits<double>::max ();
    result.jump_dist = MAX_STEPS;
    size_t di = 0;

    FPoint jump_pos[11];
    int jump_angle[11];
    int jump_jumptime[11];

    jump_pos[di] = prev_pos;
    jump_angle[di] = angle;
    jump_jumptime[di] = jumptime;
    bool jump_now = false;

    while (di < 10 && !abort) {
        if (di <= distance && !jump_now) {
            switch (def) {
            case KS_Left:
                jump_angle[di + 1] = (jump_angle[di] + ANGLES - 1) % ANGLES;
                break;
            case KS_Right:
                jump_angle[di + 1] = (jump_angle[di] + 1) % ANGLES;
                break;
            default:
                jump_angle[di + 1] = jump_angle[di];
                break;
            }
        } else {
            jump_angle[di + 1] = jump_angle[di];
        }
        jump_pos[di + 1].x = jump_pos[di].x + icos[jump_angle[di + 1]] / 2;
        jump_pos[di + 1].y = jump_pos[di].y + isin[jump_angle[di + 1]] / 2;
        if (jump_now) {
            jump_jumptime[di + 1] = JUMP_REPEAT;
            jump_now = false;
            result.jump_dist = di;
        } else {
            jump_jumptime[di + 1] = (jump_jumptime[di] <= 1) ? 0
                    : jump_jumptime[di] - 1;
        }

        if (!will_survive (jump_pos[di + 1], jump_jumptime[di + 1], head + di)) {
            if (di % 2 == 0) {
                if (jump_jumptime[di + 1] == 0) {
                    jump_now = true;
                    continue;
                }
            } else {
                if (jump_jumptime[di] == 0) {
                    jump_now = true;
                    di--;
                    result.dist--;
                    continue;
                }
            }
            return result;
        }
        result.min_target (target_distance (jump_pos[di + 1]));
        if (jump_jumptime[di + 1] <= JUMP_REPEAT - JUMP_LENGTH) {
            result.dist++;
        }
        di++;
    }

    pos = jump_pos[10];
    angle = jump_angle[10];
    jumptime = jump_jumptime[10];
    head += 10;

    for (; di < distance && di < MAX_STEPS && !abort; di += 1) {
        if (jumptime > 0)
            jumptime--;

        switch (def) {
        case KS_Left:
            angle = (angle + ANGLES - 1) % ANGLES;
            break;
        case KS_Right:
            angle = (angle + 1) % ANGLES;
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

    for (; di < MAX_STEPS && !abort; di += 2) {
        if (jumptime > 0)
            jumptime--;

        pos.x += icos[angle] / 2;
        pos.y += isin[angle] / 2;
        result.min_target (target_distance (pos));
        if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
            result.dist++;
        }
        head++;

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

inline AIGen0::Result::Result (size_t d, double t) :
    dist (d), target (t), jump_dist (MAX_STEPS) {

}

inline AIGen0::Result::Result () {

}

inline void AIGen0::Result::min_target (double value) {
    target = (target <= value) ? target : value;
}

inline bool AIGen0::Result::operator >= (const Result& other) const {
    return (dist > other.dist || (dist == other.dist && target <= other.target));
}
