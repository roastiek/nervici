/* 
 * File:   screen.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 14:07
 */

#ifndef SCREEN_H
#define	SCREEN_H

//#include <SDL.h>

#include "control.h"

/*#ifndef SDL_Surface
struct SDL_Surface;
#endif*/

class Screen : public Control {
private:
    static ControlParameters parms;

    Control* be_clicked;
    Control* mouse_target;
    Control* popup;
    Control* popup_owner;
    SDL_Surface *primary;

    void set_mouse_target (Control* value);

    void add_popup (Control* pop, Control* own);

    void remove_popup (bool restore_focus);

    void poput_lost_focus (Control* ctl);

protected:
    Screen ();

    void init_control (Control* par);

    void on_update (int x, int y, int w, int h);

    int get_screen_width () const;

    int get_screen_height () const;

    void reinitialize ();

public:
    ~Screen ();

    void process_event (SDL_Event& event);

    bool is_focusable ();

    friend class ScreenFactory;
};

class ScreenFactory {
public:
    static Screen* create (SDL_Surface* face, const ustring& name = "");
};


#endif	/* SCREEN_H */

