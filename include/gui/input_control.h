/* 
 * File:   input_control.h
 * Author: bobo
 *
 * Created on 19. červenec 2010, 21:16
 */

#ifndef INPUT_CONTROL_H
#define	INPUT_CONTROL_H

#include "control.h"

class InputControl : public Control {
private:

    struct {
        uint32_t background;
        uint32_t text;
        uint32_t foreground;
    } colors;

protected:

    InputControl (const ControlParameters& parms);

    void init_control (Control* par);

public:

    virtual void set_input_background (uint32_t value);

    virtual void set_input_text (uint32_t value);

    virtual void set_input_foreground (uint32_t value);

    virtual uint32_t get_input_background () const;

    virtual uint32_t get_input_text () const;

    virtual uint32_t get_input_foreground () const;

};

#endif	/* INPUT_CONTROL_H */

