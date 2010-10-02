/* 
 * File:   pl_info.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 15:30
 * 
 * Stores information about players, both humans and AIs.
 */

#ifndef PL_INFO_H
#define	PL_INFO_H

#include <glibmm/ustring.h>

#include "pl_type.h"
#include "ais/ai_info.h"

/*
 * Stores keys which player uses to control its nerve.
 */
struct PlKeys {
    int left;
    int right;
    int jump;
};

/*
 * All informations about a player that are needed to store.
 */
struct PlInfo {
    /*
     * nerve color
     */
    uint32_t color;

    /*
     * whether represents human or AI
     */
    PlTypes type;

    /*
     * player's name
     */
    Glib::ustring name;

    /*
     * Specific informations for human or ai player.
     */
    union {
        PlKeys keys;
        AIInfo ai;
    };

    /*
     * a sound profil of the player
     */
    Glib::ustring profil;

    /*
     * pitch of player's voice
     */
    int pitch;

    PlInfo ();

    PlInfo (uint32_t nc,
            const Glib::ustring& nn,
            bool jump,
            int distance,
            const Glib::ustring& np,
            int npi);
};

#endif	/* PL_INFO_H */

