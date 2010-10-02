/* 
 * File:   teaminfo.h
 * Author: deedrah
 *
 * Created on 21. červenec 2010, 20:39
 * 
 * Stores information about a team.
 */

#ifndef TEAMINFO_H
#define	TEAMINFO_H

#include <glibmm/ustring.h>

struct TeamInfo {
    /*
     * Team's id. It helps with mapping between players and teams. It is not 
     * saved and stay same.
     */
    uint8_t id;
    
    /*
     * Team's color.
     */
    uint32_t color;
    
    /*
     * Team's name.
     */
    Glib::ustring name;
};

#endif	/* TEAMINFO_H */

