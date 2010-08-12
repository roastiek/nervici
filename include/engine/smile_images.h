/* 
 * File:   smile_faces.h
 * Author: bobo
 *
 * Created on 23. červenec 2010, 1:49
 */

#ifndef SMILE_IMAGES_H
#define	SMILE_IMAGES_H

#include <SDL_video.h>
#include <vector>

#include "game/smyle_type.h"

struct SmileImages {
    std::vector<SDL_Surface*> eyes[ST_ham];
    std::vector<SDL_Surface*> mouths[ST_ham];
    std::vector<SDL_Surface*> hams[3];
    SDL_Surface* backs[6];
};

#endif	/* SMILE_FACES_H */

