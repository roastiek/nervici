/*
 * sound_profile.h
 *
 *  Created on: 10.9.2010
 *      Author: deedrah
 */

#ifndef SOUND_PROFILE_H_
#define SOUND_PROFILE_H_

#include <glibmm/ustring.h>

/*
 * Represents one sound profile.
 */
struct SoundProfile {
    /*
     * Sound profile's name, derived from directory name.
     */
    Glib::ustring name;
    /*
     * Whole path to sound profile's directory.
     */
    Glib::ustring directory;
};

#endif /* SOUND_PROFILE_H_ */
