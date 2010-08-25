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
#include "game/key_state.h"
#include "game/fields.h"

struct OneStep {
    FPoint pos;
    int angle;
    int jump_time;
    KeySt desicion;
};

#define MAX_STEPS 256

struct Index {
private:
    size_t value;
    
public:
    Index (size_t v = 0);
    
    operator size_t () const;
    
    Index& operator++ (int);
};

class AIGen0 {
private:
    const plid_tu id;
    
    Fields fields;
    
    std::vector<KeySt> plan;

    size_t make_plan (const FPoint& prev_pos, int prev_angle, int jumptime, plsize_tu head, 
            size_t distance, KeySt def, size_t max);
    
    bool will_survive (const FPoint& pos, int jumptime, plsize_tu head);
    
public:
    AIGen0 (plid_tu id);
    
    void calc (const FPoint& pos, int angle, int jump_time, plsize_tu head);
    
    KeySt get_next_step ();
};

#endif /* GEN0_H_ */
