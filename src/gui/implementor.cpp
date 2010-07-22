#include "gui/implementor.h"

Implementor::Implementor () :
surface (NULL),
pango_context (SDLPango_CreateContext ()),
font_color (*MATRIX_TRANSPARENT_BACK_BLACK_LETTER) {
}

Implementor::~Implementor () {
    if (surface != NULL) {
        SDL_FreeSurface (surface);
        surface = NULL;
    }
    SDLPango_FreeContext (pango_context);
}
