/*
 * sdl_screen.cpp
 *
 *  Created on: 13.9.2010
 *      Author: bobo
 */

#include <SDL.h>

#include "gui/sdl_canvas.h"

#include "gui/sdl_screen.h"

SDLScreen::SDLScreen (SDL_Surface* prim) :
    primary (prim) {

}

void SDLScreen::reinitialize () {
    set_x (0);
    set_y (0);
    set_width (primary->w);
    set_height (primary->h);
}

void SDLScreen::on_update (int x, int y, int w, int h) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    SDLCanvas* sdlcanvas = dynamic_cast<SDLCanvas*> (canvas);

    SDL_BlitSurface (sdlcanvas->get_surface (), &dest, primary, &dest);
    SDL_UpdateRects (primary, 1, &dest);
}

int SDLScreen::get_screen_width () const {
    return primary->w;
}

int SDLScreen::get_screen_height () const {
    return primary->h;
}

