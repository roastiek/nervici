/*
 * ai.h
 *
 *  Created on: 30.9.2010
 *      Author: deedrah
 *      
 * Interface for AI algorithms.     
 */

#ifndef AI_H_
#define AI_H_

#include "int_type.h"
#include "game/key_state.h"
#include "fakes/player.h"

/*
 * AI interface simulates a living player. Because in each round can a player do
 * only one command, it simply calculates and returns the best one.
 */
class AI {
public:
    virtual ~AI ();

    /*
     * Starts computing of the next best step in separeted thread.
     */
    virtual void calc (int jumptime, plsize_tu head) = 0;

    /*
     * Force to finish calculation.
     */
    virtual void finish () = 0;
    
    /*
     * Waits for calculation end and returns its result.
     */
    virtual KeySt get_next_step () = 0;

    /*
     * Creates specific AI for the player. For human player it returns NULL.
     */
    static AI* create (Player& player);
};

#endif /* AI_H_ */
