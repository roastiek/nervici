/* 
 * File:   Scrollport.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 17:12
 */

#ifndef SCROLLPORT_H
#define	SCROLLPORT_H

#include "control.h"
#include "button.h"
#include "scrollbar.h"
#include "view.h"

class _Scrollport : public _Control {
public:

    struct ScrollportPointer : public Pointer<_Scrollport, Control> {
    public:

        ScrollportPointer () : Pointer<_Scrollport, Control > (NULL) {
        }

        ScrollportPointer (_Scrollport * ctl) : Pointer<_Scrollport, Control > (ctl) {
        }

        ScrollportPointer (Control par, Control content, const ControlParameters * parms) :
        Pointer<_Scrollport, Control > (new _Scrollport ()) {
            get ()->init_control (par, parms);
            get ()->init_scrollport (content);
        }

    };

    typedef ScrollportPointer Scrollport;

private:
    View view;
    Control content;
    Scrollbar bar;

    static ScrollbarParameters bar_parms;
    static ControlParameters view_parms;

    void bar_value_changed (Control ctl, int value);

    void content_height_changed (Control ctl, int value);

    void child_focus_gained (Control ctl);

    void child_focus_lost (Control ctl);

protected:
    _Scrollport ();

    void init_control (Control par, const ControlParameters* parms);

    void init_scrollport (Control content);

    void reinitialize ();

public:
    bool is_focusable () const;

    void set_content (Control value);

    Control get_content () const;
};

typedef _Scrollport::Scrollport Scrollport;

#endif	/* SCROLLPORT_H */

