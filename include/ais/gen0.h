/*
 * gen0.h
 *
 *  Created on: 24.8.2010
 *      Author: bobo
 */

#ifndef GEN0_H_
#define GEN0_H_

#include <vector>

#include "fpoint.h"
#include "point.h"
#include "ai.h"
#include "ai_info.h"

class AIGen0 : public AI {
private:
    struct Result {
        size_t dist;

        double target;

        size_t jump_dist;
        
        Result (size_t d, double t);
        
        Result ();
        
        void min_target (double value);
        
        bool operator >= (const Result& other) const;
    };

    Player& player;

    KeySt plan;

    Point target;
    
    volatile bool ready;
    
    volatile bool abort;
    
    int calc_jumptime;
    
    plsize_tu calc_head;
    
    smileid_tu smile_target;
    
    double best_target_dist;
    
    int target_tries;
    
    AIGen0Info info;
    
    bool will_survive (const FPoint& pos, int jumptime, plsize_tu head);

    double target_distance (const FPoint& pos);

    void work ();
    
    void check_target ();
    
    void random_target ();
    
    smileid_tu find_closest_smile ();
    
    Result test_plan (const FPoint& prev_pos, int angle, int jumptime,
            plsize_tu head, KeySt def, size_t distance);
    
public:
    AIGen0 (Player& player, const AIGen0Info& info);

    void calc (int jumptime, plsize_tu head);

    void finish ();
    
    KeySt get_next_step ();
};

#endif /* GEN0_H_ */
