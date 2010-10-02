/*
 * ai_info.h
 *
 *  Created on: 30.9.2010
 *      Author: deedrah
 *      
 * This header contains structures for storing differents AIs settings.       
 */

#ifndef AI_INFO_H_
#define AI_INFO_H_

/*
 * Defines constant symbolizing AI gen 0.
 */
#define AI_GEN_0 0

/*
 * AI gen 0 options.
 */
struct AIGen0Info {
    /*
     * If Ai can jump.
     */
    bool jump;
    
    /*
     * Sets search distance for pathing algorithm. Maximum is 160. Higher value 
     * should be better, but needs more computing time.
     */
    uint8_t distance;
};

/*
 * Generic structure for AI options.
 */
struct AIInfo {
    /*
     * Type of AI stored in this structure.
     */
    uint8_t gen;
    
    union {
        AIGen0Info gen0;
    };
};

#endif /* AI_INFO_H_ */
