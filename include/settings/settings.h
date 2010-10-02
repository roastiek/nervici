/*
 * settings.h
 *
 *  Created on: 20.8.2010
 *      Author: deedrah
 *     
 * Provides colletions of all Setting objects.    
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "fakes/setting.h"

/*
 * All types of setting files, except of the last one which represents total 
 * count of setting files.
 */
enum SettingFiles {
    SF_app, SF_game, SF_players, SF_ais, SF_teams, SF_count
};

/*
 * Using sigleton pattern
 * Settings class group toghether all setting files of the appilication. Options
 * are diveded into separeted files, because some of the options will not be 
 * changed so often as the others.
 */
class Settings {
private:
    Setting* settings[SF_count];

    static Settings instance;

    Settings ();

    ~Settings ();

public:
    /*
     * Loads all setting files.
     */    
    void load ();

    /*
     * Saves all setting files.
     */
    void save ();

    /*
     * Returns app setting object. It is used to store information about sound 
     * and graphics.
     */
    Setting& app ();

    /*
     * Returns players setting object. It is used to store information about 
     * players.
     */
    Setting& players ();

    /*
     * Returns ais setting object. It is used to store information about ais.
     */
    Setting& ais ();

    /*
     * Returns game setting object. It is used to store information about the 
     * game (choosen players, mod, mod options). 
     */
    Setting& game ();

    /*
     * Returns teams setting object. It is used to store information about 
     * teams.
     */
    Setting& teams ();

    static Settings& get_instance ();
};

extern Settings& settings;

inline Setting& Settings::app () {
    return *settings[SF_app];
}

inline Setting& Settings::game () {
    return *settings[SF_game];
}

inline Setting& Settings::players () {
    return *settings[SF_players];
}

inline Setting& Settings::ais () {
    return *settings[SF_ais];
}

inline Setting& Settings::teams () {
    return *settings[SF_teams];
}

inline Settings& Settings::get_instance () {
    return instance;
}


#endif /* SETTINGS_H_ */
