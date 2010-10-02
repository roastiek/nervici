/* 
 * File:   music_type.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 15:51
 */

#ifndef MUSIC_TYPE_H
#define	MUSIC_TYPE_H

/*
 * Represent different music types.
 */
enum MusicType {
    /*
     * Music for a game shorter than 1 min 30 sec.
     */
    MT_GameShort,
    /*
     * Music for a game longer than 1 min 30 sec.
     */
    MT_GameLong,
    /*
     * Music for a menu.
     */
    MT_Menu,
    /*
     * Music for a statistic.
     */
    MT_Stat,
    /*
     * Count of music types.
     */
    MT_Count
};

#endif	/* MUSIC_TYPE_H */

