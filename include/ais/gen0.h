/*
 * gen0.h
 *
 *  Created on: 24.8.2010
 *      Author: bobo
 */

#ifndef GEN0_H_
#define GEN0_H_

#include <vector>

#include "int_type.h"
#include "fpoint.h"
#include "point.h"
#include "game/key_state.h"
#include "game/fields.h"

struct OneStep {
    FPoint pos;
    int angle;
    int jump_time;
    KeySt desicion;
};

#define MAX_STEPS 80

struct Index {
private:
    size_t value;

public:
    Index (size_t v = 0);

    operator size_t () const;

    Index& operator++ (int);
};

struct Result {
    size_t dist;

    double target;

    Result (size_t d, double t) :
        dist (d), target (t) {

    }
    
    Result () {
        
    }
    
    /*inline bool operator > (const Result& other) {
        return (dist > other.dist || (dist == other.dist && target < other.target));
    }*/

    inline bool operator >= (const Result& other) {
        return (dist > other.dist || (dist == other.dist && target <= other.target));
    }

    /*inline bool operator < (const Result& other) {
        return (dist < other.dist || (dist == other.dist && target < other.target));
    }*/
};

class AIGen0 {
private:
    const plid_tu id;

    Fields fields;

    std::vector<KeySt> plan;

    size_t barier[8];

    bool can_jump;

    Point target;
    
    double shortes;

    void make_shortes_plan (const FPoint& prev_pos, int prev_angle,
            int jumptime, plsize_tu head, size_t distance);

    Result make_short_plan (const FPoint& prev_pos, int prev_angle,
            int jumptime, plsize_tu head, size_t distance, KeySt def);

    Result make_plan (const FPoint& prev_pos, int prev_angle, int jumptime,
            plsize_tu head, size_t distance, KeySt def, size_t max);

    bool will_survive (const FPoint& pos, int jumptime, plsize_tu head);

    double target_distance (const FPoint& pos);

    void clear_barier (int from);
    
    Point random_target ();
    
public:
    AIGen0 (plid_tu id);

    void calc (const FPoint& pos, int angle, int jumptime, plsize_tu head);

    KeySt get_next_step ();
};

#endif /* GEN0_H_ */
