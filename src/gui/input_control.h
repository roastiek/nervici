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
        Uint32 background;
        Uint32 text;
        Uint32 foreground;
    } colors;

protected:

    InputControl (const ControlParameters& parms);

    void init_control (Control* par);

public:

    virtual void set_input_background (Uint32 value);

    virtual void set_input_text (Uint32 value);

    virtual void set_input_foreground (Uint32 value);

    virtual Uint32 get_input_background () const;

    virtual Uint32 get_input_text () const;

    virtual Uint32 get_input_foreground () const;

};

#endif	/* INPUT_CONTROL_H */

