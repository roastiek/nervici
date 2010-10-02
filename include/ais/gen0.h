/*
 * gen0.h
 *
 *  Created on: 24.8.2010
 *      Author: deedrah
 *      
 * Generation 0 AI.     
 */

#ifndef GEN0_H_
#define GEN0_H_

#include <vector>

#include "fpoint.h"
#include "point.h"
#include "ai.h"
#include "ai_info.h"

/*
 * This Ai version checks everytime some predefinded paths and chooses the best.
 * The paths are:
 *      1. Direct
 *      2. Truns 22,5 degrees to left and direct 
 *      3. Truns 22,5 degrees to right and direct 
 *      4. Truns 90 degrees to left and direct 
 *      5. Truns 90 degrees to right and direct 
 *      6. Truns 180 degrees to left and direct 
 *      7. Truns 180 degrees to left and direct
 
 * Jump is possible only in the first ten steps. Additionally it calculates 
 * the nearest distance to the target on each path. The best path is the one 
 * with the longest survive distance and the nearest target distance.       
 */
class AIGen0 : public AI {
private:
    /*
     * Represents properties of one path
     */
    struct Result {
        /*
         * Maximal survive distance
         */
        size_t dist;

        /*
         * Minimal target distance
         */
        double target;

        /*
         * If plan for path contains jump, which step is it. Value 0 indicates
         * that AI should jump this round.
         */
        size_t jump_dist;
        
        Result (size_t d, double t);
        
        Result ();
        
        void min_target (double value);
        
        bool operator >= (const Result& other) const;
    };

    /*
     * Player using this AI.
     */
    Player& player;

    /*
     * PLan for next round.
     */
    KeySt plan;

    /*
     * Coordinates, where nerve tries to go. They are random numbers or position
     * os some good smile.
     */
    Point target;
    
    /*
     * Indicates, that plan has been successfully calculated. Used for 
     * synchronization between main and ai thread.
     */
    volatile bool ready;
    
    /*
     * Indicates, that caluclation should be terminated. 
     */
    volatile bool abort;
    
    /*
     * State of player's actual jump timer.
     */
    int calc_jumptime;
    
    /*
     * Position of player's head.
     */
    plsize_tu calc_head;
    
    /*
     * Contains ID of targeted smile. Value same as smile counts represents no
     * smile target.
     */
    smileid_tu smile_target;
    
    /*
     * The closest distance to same target, that nerve has ever been.
     */
    double best_target_dist;
    
    /*
     * If actual distance to tatget is larger as best_target_distance, 
     * target_tries will incrase. If target_tries is higher than 
     * MAX_TARGET_TRIES, new target will be choosen.
     */
    int target_tries;
    
    /*
     * Settings for AI.
     */
    AIGen0Info info;
    
    /*
     * Tests if a position is safe. 
     */
    bool will_survive (const FPoint& pos, int jumptime, plsize_tu head);

    /*
     * Returns distance to the target from a position.
     */
    double target_distance (const FPoint& pos);

    /*
     * Caclulates the best plan. It is running in own thread.
     */
    void work ();
    
    /*
     * Checks if target is still valid and if not a new target will be choosen.
     */
    void check_target ();
    
    /*
     * Generates random target;
     */
    void random_target ();
    
    /*
     * Search for closest good smile.
     */
    smileid_tu find_closest_smile ();
    
    /*
     * Calculates properties of one of paths.
     *      def - default plan
     *      distance - length of default plan
     */
    Result test_plan (const FPoint& prev_pos, int angle, int jumptime,
            plsize_tu head, KeySt def, size_t distance);
    
public:
    AIGen0 (Player& player, const AIGen0Info& info);

    void calc (int jumptime, plsize_tu head);

    void finish ();
    
    KeySt get_next_step ();
};

#endif /* GEN0_H_ */
