/*
 * settings.h
 *
 *  Created on: 20.8.2010
 *      Author: bobo
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <vector>

#include "fakes/setting.h"

class Settings {
private:
    std::vector<Setting*> settings;

    static Settings instance;

    Settings ();

    ~Settings ();

public:

    void load ();

    void save ();

    Setting& app ();

    Setting& players ();

    Setting& ais ();

    Setting& game ();

    Setting& teams ();

    static Settings& get_instance ();
};

extern Settings& settings;

inline Setting& Settings::app () {
    return *settings[0];
}

inline Setting& Settings::game () {
    return *settings[1];
}

inline Setting& Settings::players () {
    return *settings[2];
}

inline Setting& Settings::ais () {
    return *settings[3];
}

inline Setting& Settings::teams () {
    return *settings[4];
}

inline Settings& Settings::get_instance () {
    return instance;
}


#endif /* SETTINGS_H_ */
