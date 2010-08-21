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

public:
    ~Settings ();

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

#endif /* SETTINGS_H_ */
