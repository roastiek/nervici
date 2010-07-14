/* 
 * File:   screen.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 14:07
 */

#ifndef SCREEN_H
#define	SCREEN_H

#include "control.h"

class Screen : public Control {
private:
    Control* be_cliked;
    Control* mouse_target;
    Control* popup;
    Control* popup_owner;
    SDL_Surface *primary;

    void set_mouse_target (Control* value);

    void add_popup (Control* pop, Control* own);

    void remove_popup (bool restore_focus);

    void poput_lost_focus  (Control* ctl);

protected:
    void on_update (int x, int y, int w, int h);
    //void on_update_child (int x, int y, int w, int h);

public:

    Screen ();
    ~Screen ();

    void process_event (SDL_Event& event);
};

#endif	/* SCREEN_H */

