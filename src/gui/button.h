/* 
 * File:   button.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 13:24
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include "control.h"

struct ButtonParameters : public ControlParameters {
    const ustring text;
    ButtonParameters (float nx, float ny, float nw, float nh, float nf,
            const ustring& nt, const ustring& nn = "");
};

class _Button : public _Control {
public:
    struct ButtonPointer: public Pointer<_Button, _Control> {
    public:
        ButtonPointer (): Pointer<_Button, _Control>() {
        }

        ButtonPointer (_Button* it): Pointer<_Button, _Control>(it) {
        }

        ButtonPointer (ControlPointer& par, const ButtonParameters* parms):
        Pointer<_Button, _Control>(new _Button()){
            get()->init_control (par, parms);
            get()->init_button (parms);
        }
    };

    typedef ButtonPointer Button;

private:
    ustring text;

protected:
    virtual void init_button (const ButtonParameters* parms);

    void paint ();

    void on_focus_gained ();

    void on_focus_lost ();

public:
    //static Button* create (Control* par, const ButtonParameters* parms);

    void set_text (const ustring& value);

    const ustring& get_text () const;
};

typedef _Button::Button Button;

#endif	/* BUTTON_H */

