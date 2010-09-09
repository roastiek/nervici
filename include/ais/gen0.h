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
#include "fakes/player.h"

struct OneStep {
    FPoint pos;
    int angle;
    int jump_time;
    KeySt desicion;
};

#define MAX_STEPS 160

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

    size_t jump_dist;
    
    Result (size_t d, double t) :
        dist (d), target (t), jump_dist (MAX_STEPS) {

    }
    
    Result () {
        
    }
    
    inline void min_target (double value) {
        target = (target <= value) ? target : value; 
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
    Player& player;

    Fields fields;

    KeySt plan;

    size_t barier[8];

    bool can_jump;

    Point target;
    
    double shortes;

    bool ready;
    
    bool abort;
    
    FPoint calc_pos;
    
    int calc_angle;
    
    int calc_jumptime;
    
    plsize_tu calc_head;
    
    smileid_tu smile_target;
    
    double best_target_dist;
    
    int target_tries;
    
    void make_shortes_plan (const FPoint& prev_pos, int prev_angle,
            int jumptime, plsize_tu head, size_t distance);

    Result make_plan (const FPoint& prev_pos, int prev_angle, int jumptime,
            plsize_tu head, size_t distance, KeySt def, size_t max);

    bool will_survive (const FPoint& pos, int jumptime, plsize_tu head);

    double target_distance (const FPoint& pos);

    void clear_barier (int from);
    
    void work ();
    
    void check_target ();
    
    void next_target ();

    void random_target ();
    
    smileid_tu find_closest_smile ();
    
    Result test_plan (const FPoint& prev_pos, int angle, int jumptime,
            plsize_tu head, KeySt def, size_t distance);
    
public:
    AIGen0 (Player& player);

    void calc (const FPoint& pos, int angle, int jumptime, plsize_tu head);

    KeySt get_next_step ();
};

#endif /* GEN0_H_ */
