/* 
 * File:   Implementor.h
 * Author: bobo
 *
 * Created on 16. červenec 2010, 1:35
 */

#ifndef IMPLEMENTOR_H
#define	IMPLEMENTOR_H

#include <SDL.h>
#include <SDL_Pango.h>

struct Implementor {
    SDL_Surface* surface;
    SDLPango_Context* pango_context;
    SDLPango_Matrix font_color;

    Implementor () : surface (NULL), pango_context (SDLPango_CreateContext ()), font_color (*MATRIX_TRANSPARENT_BACK_WHITE_LETTER) {
    }
};

#endif	/* IMPLEMENTOR_H */

