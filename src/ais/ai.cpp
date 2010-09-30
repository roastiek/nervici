/*
 * ai.cpp
 *
 *  Created on: 30.9.2010
 *      Author: bobo
 */

#include "game/player.h"
#include "settings/pl_type.h"
#include "settings/pl_info.h"
#include "ais/gen0.h"

#include "ais/ai.h"

AI::~AI () {

}

AI* AI::create (Player& player) {
    if (player.info.type != PT_AI)
        return NULL;

    switch (player.info.ai.gen) {
    case AI_GEN_0:
        return new AIGen0 (player, player.info.ai.gen0);
    }

    return NULL;
}
