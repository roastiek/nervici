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
    Screen (), off_face (NULL), primary (prim), ignore_updates (false),
            abort (false) {

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
        if (!ignore_updates) {
            SDLClip clip (off_face, area->w, area->h, area->x, area->y);
            update (&clip);
        }

        delete area;
        break;
    }
    default:
        Screen::process_event (event);
        break;
    }
}

bool SDLScreen::process_events () {
    SDL_Event event;

    while (SDL_PollEvent (&event)) {
        switch (event.type) {
        case SDL_QUIT:
            abort = true;
            break;
        default:
            process_event (event);
            break;
        }
    }
    return !is_aborted ();
}

bool SDLScreen::is_aborted () const {
    return abort;
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

void SDLScreen::set_surface (SDL_Surface* value) {
    primary = value;
    set_ignore_updates (true);
    reinitialize ();
    reinitialize_children ();
    process_events ();
    set_ignore_updates (false);
}

void SDLScreen::set_ignore_updates (bool value) {
    if (ignore_updates != value) {
        ignore_updates = value;
        if (!ignore_updates) {
            SDL_Rect *area = new SDL_Rect;
            SDL_Event event;

            area->x = 0;
            area->y = 0;
            area->w = get_width ();
            area->h = get_height ();

            event.user.type = E_PAINT;
            event.user.data1 = area;
            SDL_PushEvent (&event);
        }
    }
}

SDLScreen* SDLScreenFactory::create (SDL_Surface* primary) {
    SDLScreen* result = new SDLScreen (primary);
    Control::screen = result;
    result->set_name ("nervici");
    result->init_control (NULL);

    return result;
}
