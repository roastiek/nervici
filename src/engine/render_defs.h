/* 
 * File:   render_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 17:22
 */

#ifndef RENDER_DEFS_H
#define	RENDER_DEFS_H

struct GameScreen {
    SDL_Rect score;
    SDL_Rect team;
    SDL_Rect round;
    SDL_Rect semafor;
    SDL_Rect status;
    SDL_Rect timer;
    SDL_Rect statustext;
    SDL_Rect playerground;
};

struct ScreenSet {
    int width;
    int height;
    int fullscreen;
};

struct PlayerSurfaces {
    SDL_Surface *face;
    SDL_Surface *numbers;
};

#endif	/* RENDER_DEFS_H */

