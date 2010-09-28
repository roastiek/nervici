/*
 * listbox_parameters.h
 *
 *  Created on: 28.9.2010
 *      Author: bobo
 */

#ifndef LISTBOX_PARAMETERS_H_
#define LISTBOX_PARAMETERS_H_

struct ListboxParameters: public ControlParameters {
    const float item_height;

    ListboxParameters (float nx,
            float ny,
            float nw,
            float nh,
            float nf,
            float nih);
};

inline ListboxParameters::ListboxParameters (float nx,
        float ny,
        float nw,
        float nh,
        float nf,
        float nih) :
    ControlParameters (nx, ny, nw, nh, nf), item_height (nih) {
}


#endif /* LISTBOX_PARAMETERS_H_ */
