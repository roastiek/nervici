/* 
 * File:   smile_faces.h
 * Author: bobo
 *
 * Created on 23. červenec 2010, 1:49
 */

#ifndef SMILE_IMAGES_H
#define	SMILE_IMAGES_H

#include <vector>

#include "game/smile_type.h"
#include "fakes/sdl_decl.h"

struct SmileImages {
    std::vector<SDL_Surface*> eyes[ST_cham];
    std::vector<SDL_Surface*> mouths[ST_cham];
    std::vector<SDL_Surface*> hams[3];
    SDL_Surface* backs[6];
};

#endif	/* SMILE_FACES_H */

