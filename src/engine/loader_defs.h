/* 
 * File:   loader_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 17:25
 */

#ifndef LOADER_DEFS_H
#define	LOADER_DEFS_H

#include "gui/canvas.h"

enum ImageType {
    IMT_Numbers, IMT_Timer, IMT_Semafor, IMT_Heart, IMT_Count
};

inline ImageType operator ++(ImageType& it, int) {
    it = (ImageType) (it + 1);
    return it;
}

enum FontType {
    FNT_Mono20, FNT_Mono100, FNT_Count
};

typedef Canvas* SmileSettingImages[21];

#endif	/* LOADER_DEFS_H */

