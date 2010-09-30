/* 
 * File:   screen.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 14:07
 */

#ifndef SCREEN_H
#define	SCREEN_H

#include "control.h"

class Screen: public Control {
private:
    Control* be_clicked;

    Control* mouse_target;

    Control* popup;

    Control* popup_owner;
    
    static ControlParameters parms;

    void set_mouse_target (Control* value);

    void add_popup (Control* pop, Control* own);

    void remove_popup (bool restore_focus);

protected:
    Screen ();

    virtual void init_control (Control* par);
    
    void paint ();

    virtual void process_event (const SDL_Event& event);

public:
    ~Screen ();

    virtual bool process_events () = 0;
    
    virtual void set_ignore_updates (bool value) = 0;

    bool is_focusable () const;
    
    virtual bool is_aborted () const = 0;
};

#endif	/* SCREEN_H */

