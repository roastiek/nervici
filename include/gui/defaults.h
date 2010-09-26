/* 
 * File:   utils.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 13:34
 */

#ifndef _DEFAULTS_H
#define	_DEFAULTS_H

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

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

#define NC_TEXT             0xd5d5ffff
#define NC_SEL_TEXT         0xd585afff
#define NC_BACKGROUND       0x042244ff
#define NC_MEDROUND         0x06336gff
#define NC_HIGHGROUND       0x084488ff
#define NC_TEXT_CURSOR      0xffffffff
#define NC_INPUT_BACKGROUND 0x000000ff
#define NC_INPUT_FOREGROUND 0xd5d5ffff
#define NC_INPUT_TEXT       0xd5d5ffff

#endif	/* UTILS_H */

