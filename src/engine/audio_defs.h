/* 
 * File:   audio_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 17:23
 */

#ifndef AUDIO_DEFS_H
#define	AUDIO_DEFS_H

enum EffectType {
    ET_Hop, ET_Jau, ET_Self, ET_SmilePlus, ET_SmileMinus, ET_Wall
};

enum MusicType {
    MT_Short, MT_Long, MT_Menu, MT_Stat, MT_Count
};

inline MusicType operator++(MusicType &mt, int) {
    mt = (MusicType) (mt + 1);
    return mt;
}

#endif	/* AUDIO_DEFS_H */

