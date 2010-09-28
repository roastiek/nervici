/*
 * scrollbar_parameters.h
 *
 *  Created on: 28.9.2010
 *      Author: bobo
 */

#ifndef SCROLLBAR_PARAMETERS_H_
#define SCROLLBAR_PARAMETERS_H_

#include "control.h"

struct ScrollbarParameters: public ControlParameters {
    const float small_step;
    const float big_step;
    ScrollbarParameters (float nx,
            float ny,
            float nw,
            float nh,
            float nf,
            float nss,
            float nbs);
};

inline ScrollbarParameters::ScrollbarParameters (float nx,
        float ny,
        float nw,
        float nh,
        float nf,
        float nss,
        float nbs) :
    ControlParameters (nx, ny, nw, nh, nf), small_step (nss), big_step (nbs) {
}

#endif /* SCROLLBAR_PARAMETERS_H_ */
