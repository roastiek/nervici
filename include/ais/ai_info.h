/*
 * ai_info.h
 *
 *  Created on: 30.9.2010
 *      Author: bobo
 */

#ifndef AI_INFO_H_
#define AI_INFO_H_

#define AI_GEN_0 0

struct AIGen0Info {
    bool jump;
    uint8_t distance;
};

struct AIInfo {
    uint8_t gen;
    union {
        AIGen0Info gen0;
    };
};

#endif /* AI_INFO_H_ */
