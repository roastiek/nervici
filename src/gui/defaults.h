/* 
 * File:   utils.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 13:34
 */

#ifndef _DEFAULTS_H
#define	_DEFAULTS_H

/*#include <SDL.h>
#include <SDL_Pango.h>*/

//using namespace std;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

/*#define _MAKE_FNT_NAME(family, size)   family " " #size
#define FORM_FONT _MAKE_FNT_NAME ("Sans", 12 * 1024 / SCREEN_WIDTH)*/

//extern bool labort;

//#define FRAME_WIDHT 2
#define C_SCREEN_BACKGROUND 0x000000ff
#define C_BACKGROUND        0x201008ff
#define C_FOREGROUND        0xff8040ff
#define C_TEXT              0xff8040ff
#define C_SEL_BACKGROUND    0xff8040ff
#define C_SEL_FOREGROUND    0x000000ff
#define C_FOC_FOREGROUND    0xffff40ff
#define C_FILL              0xc06020ff
#define C_TEXT_CURSOR       0xff0000ff
#define C_INPUT_BACKGROUND  0x000000ff
#define C_INPUT_FOREGROUND  0x80ff40ff
#define C_INPUT_TEXT        0x80ff40ff

#define C_DIS_FILL          0xa0a0a0ff
#define C_DIS_FOREGROUND    0xe0e0e0ff

#define E_PAINT         SDL_USEREVENT
#define E_SHOW_POPUP    SDL_USEREVENT + 1
#define E_HIDE_POPUP    SDL_USEREVENT + 2

enum HorizontalAling {
    HA_left, HA_center, HA_right
};

enum VerticalAling {
    VA_top, VA_center, VA_bottom
};

#endif	/* UTILS_H */

