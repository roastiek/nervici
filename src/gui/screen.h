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

#ifndef SDL_Surface
struct SDL_Surface;
#endif

class _Screen : public _Control {
public:

    struct ScreenPointer : public Pointer<_Screen, Control> {

        ScreenPointer () : Pointer<_Screen, Control> (NULL) {
        }

        ScreenPointer (_Screen* ctl) : Pointer<_Screen, Control> (ctl) {
        }

        ScreenPointer (SDL_Surface* face, const ustring& name = "") :
        Pointer<_Screen, Control> (new _Screen ()) {
            get ()->init_screen (face, name);
        }
    };

    typedef ScreenPointer Screen;

private:
    Control be_clicked;
    Control mouse_target;
    Control popup;
    Control popup_owner;
    SDL_Surface *primary;

    void set_mouse_target (Control value);

    void add_popup (Control pop, Control own);

    void remove_popup (bool restore_focus);

    void poput_lost_focus (Control ctl);

protected:
    _Screen ();

    void init_screen (SDL_Surface* face, const ustring& nn);

    void on_update (int x, int y, int w, int h);

    int get_screen_width () const;

    int get_screen_height () const;

    void reinitialize ();

public:
    //static Screen* create (SDL_Surface* face, const ustring& name = "");

    /*Screen (SDL_Surface *face, const ustring& name = "");*/

    ~_Screen ();

    void process_event (SDL_Event& event);

};

typedef _Screen::Screen Screen;

#endif	/* SCREEN_H */

