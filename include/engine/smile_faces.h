/* 
 * File:   smile_faces.h
 * Author: bobo
 *
 * Created on 23. červenec 2010, 1:49
 */

#ifndef SMILE_FACES_H
#define	SMILE_FACES_H

#include <SDL_video.h>
#include <vector>
using namespace std;


#include "game/smyle_type.h"

struct SmileFaces {
    vector<SDL_Surface*> eyes[ST_count];
    vector<SDL_Surface*> mouths[ST_count];
    SDL_Surface* backs[6];
};

#endif	/* SMILE_FACES_H */

