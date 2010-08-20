/* 
 * File:   teaminfo.h
 * Author: bobo
 *
 * Created on 21. červenec 2010, 20:39
 */

#ifndef TEAMINFO_H
#define	TEAMINFO_H

#include <glibmm/ustring.h>

struct TeamInfo {
    uint8_t id;
    uint32_t color;
    Glib::ustring name;
};



#endif	/* TEAMINFO_H */

