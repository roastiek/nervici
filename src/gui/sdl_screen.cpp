/*
 * sdl_screen.cpp
 *
 *  Created on: 13.9.2010
 *      Author: bobo
 */

#include <SDL.h>

#include "logger.h"
#include "gui/sdl_canvas.h"

#include "gui/sdl_screen.h"

SDLScreen::SDLScreen (SDL_Surface* prim) :
    Screen (new SDLClip (prim, 0, 0, prim->w, prim->h, 0, 0)) {

}

void SDLScreen::reinitialize () {
    set_x (0);
    set_y (0);
    set_width (primary->get_width ());
    set_height (primary->get_height ());
}

void SDLScreen::on_update (int x, int y, int w, int h) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    //SDLCanvas* sdlcanvas = dynamic_cast<SDLCanvas*> (canvas);
    SDLClip* sdlclip = dynamic_cast<SDLClip*> (primary);

    /*SDL_BlitSurface (sdlcanvas->get_surface (), &dest, sdlclip->get_surface (),
            &dest);*/
    SDL_UpdateRects (sdlclip->get_surface (), 1, &dest);
    //SDL_UpdateRect(sdlclip->get_surface (), 0, 0, 1024, 768);
}

int SDLScreen::get_screen_width () const {
    return primary->get_width ();
}

int SDLScreen::get_screen_height () const {
    return primary->get_height ();
}

