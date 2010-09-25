/*
 * sdl_screen.cpp
 *
 *  Created on: 13.9.2010
 *      Author: bobo
 */

#include <SDL.h>

#include "logger.h"
#include "gui/defaults.h"
#include "gui/sdl_canvas.h"

#include "gui/sdl_screen.h"

SDLScreen::SDLScreen (SDL_Surface* prim) :
    Screen (), off_face (NULL), primary (prim) {

}

void SDLScreen::reinitialize () {
    set_x (0);
    set_y (0);
    set_width (primary->w);
    set_height (primary->h);
    if (off_face != NULL) {
        SDL_FreeSurface (off_face);
        off_face = NULL;
    }
    off_face = SDL_CreateRGBSurface (SDL_HWSURFACE,
        get_width (),
        get_height (),
        32,
        0xff,
        0xff00,
        0xff0000,
        0x000000);
}

void SDLScreen::process_event (const SDL_Event& event) {
    switch (event.type) {
    case E_PAINT: {
        SDL_Rect* area = static_cast<SDL_Rect*> (event.user.data1);
        SDLClip clip (off_face, area->w, area->h, area->x, area->y);

        update (&clip);

        delete area;
        break;
    }
    default:
        Screen::process_event (event);
        break;
    }
}

void SDLScreen::update (Clip* scrvas) {
    Screen::update (scrvas);

    SDL_Rect dest;

    dest.x = scrvas->get_x ();
    dest.y = scrvas->get_y ();
    dest.w = scrvas->get_width ();
    dest.h = scrvas->get_height ();

    SDL_BlitSurface (off_face, &dest, primary, &dest);
    SDL_UpdateRects (primary, 1, &dest);
}

int SDLScreen::get_screen_width () const {
    return primary->w;
}

int SDLScreen::get_screen_height () const {
    return primary->h;
}

