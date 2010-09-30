/*
 * sdl_sceen.h
 *
 *  Created on: 13.9.2010
 *      Author: bobo
 */

#ifndef SDL_SCREEN_H_
#define SDL_SCREEN_H_

#include "screen.h"
#include "fakes/sdl_decl.h"

class SDLScreen: public Screen {
private:
    SDL_Surface* off_face;
    
    SDL_Surface* primary;
    
    bool ignore_updates;
    
protected:
    void reinitialize ();

    void update (Clip* scrvas);

//    int get_screen_width () const;

//    int get_screen_height () const;

public:

    SDLScreen (SDL_Surface* primary);

    void process_event (const SDL_Event& event);

    void set_surface (SDL_Surface* value);
    
    void set_ignore_updates (bool value);

    friend class SDLScreenFactory;
};

class SDLScreenFactory {
public:
    static SDLScreen* create (SDL_Surface* primary);
};

#endif /* SDL_SCREEN_H_ */
