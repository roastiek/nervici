/* 
 * File:   music_type.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 15:51
 */

#ifndef MUSIC_TYPE_H
#define	MUSIC_TYPE_H

enum MusicType {
    MT_Game, MT_Menu, MT_Stat, MT_Count
};

inline MusicType operator++(MusicType &mt, int) {
    mt = (MusicType) (mt + 1);
    return mt;
}

#endif	/* MUSIC_TYPE_H */

