/*
 * settings.cpp
 *
 *  Created on: 20.8.2010
 *      Author: bobo
 */

#include "logger.h"
#include "settings/setting.h"
#include "settings/settings.h"

Settings Settings::instance;

Settings& settings = Settings::get_instance ();

Settings::Settings () {
    settings.push_back (new Setting ("nervici.conf"));
    settings.push_back (new Setting ("game.conf"));
    settings.push_back (new Setting ("players.conf"));
    settings.push_back (new Setting ("plastiks.conf"));
    settings.push_back (new Setting ("teams.conf"));
}

Settings::~Settings () {
    logger.fineln ("freeing settings");
    for (size_t si = 0; si < settings.size (); si++) {
        delete settings[si];
    }
    settings.clear ();
}

void Settings::load () {
    logger.fineln ("loading settings");
    for (size_t si = 0; si < settings.size (); si++) {
        settings[si]->load ();
    }
}

void Settings::save () {
    logger.fineln ("saving settings");
    for (size_t si = 0; si < settings.size (); si++) {
        settings[si]->save ();
    }
}

