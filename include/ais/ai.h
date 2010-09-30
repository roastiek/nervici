/*
 * ai.h
 *
 *  Created on: 30.9.2010
 *      Author: bobo
 */

#ifndef AI_H_
#define AI_H_

#include "int_type.h"
#include "game/key_state.h"
#include "fakes/player.h"

class AI {
public:
    virtual ~AI ();

    virtual void calc (int jumptime, plsize_tu head) = 0;

    virtual void finish () = 0;

    virtual KeySt get_next_step () = 0;

    static AI* create (Player& player);
};

#endif /* AI_H_ */
