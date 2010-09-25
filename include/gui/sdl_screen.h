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
    
protected:
    void reinitialize ();

    void update (Clip* scrvas);

    int get_screen_width () const;

    int get_screen_height () const;

public:

    SDLScreen (SDL_Surface* primary);

    void process_event (const SDL_Event& event);

    friend class SDLRender;
};

#endif /* SDL_SCREEN_H_ */
