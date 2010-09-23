/*
 * sdl_sceen.h
 *
 *  Created on: 13.9.2010
 *      Author: bobo
 */

#ifndef SDL_SCREEN_H_
#define SDL_SCREEN_H_

#include "screen.h"

class SDLScreen: public Screen {

protected:
    void reinitialize ();

    void on_update (int x, int y, int w, int h);

    int get_screen_width () const;

    int get_screen_height () const;

public:

    SDLScreen (SDL_Surface* primary);

    friend class SDLRender;
};

#endif /* SDL_SCREEN_H_ */
