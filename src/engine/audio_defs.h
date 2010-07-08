/* 
 * File:   audio_defs.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 17:23
 */

#ifndef AUDIO_DEFS_H
#define	AUDIO_DEFS_H

#include <string>
#include <al.h>

using namespace std;

#define WAVS_COUNT 8

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

struct SoundProfile {
    string name;
    string directory;
    ALuint buffers[WAVS_COUNT];
};


struct AudioSetting {
    int sound;
    int music;
    int buffer;
    int bfrs_count;
};

struct MusicFile {
    string filename;
    int played;
};

struct PlAudio {
    int prof;
    ALuint source;
    int r;
};

#endif	/* AUDIO_DEFS_H */

