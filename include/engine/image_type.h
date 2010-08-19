/* 
 * File:   image_type.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 15:56
 */

#ifndef IMAGE_TYPE_H
#define	IMAGE_TYPE_H

enum ImageType {
    IMT_Numbers, IMT_Timer, IMT_Semafor, IMT_Heart, IMT_Stats, IMT_Count
};


inline ImageType operator ++(ImageType& it, int) {
    it = (ImageType) (it + 1);
    return it;
}


#endif	/* IMAGE_TYPE_H */

