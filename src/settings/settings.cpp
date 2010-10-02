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
    settings[SF_app] = new Setting ("nervici.conf");
    settings[SF_game] = new Setting ("game.conf");
    settings[SF_players] = new Setting ("players.conf");
    settings[SF_ais] = new Setting ("plastiks.conf");
    settings[SF_teams] = new Setting ("teams.conf");
}

Settings::~Settings () {
    logger.fineln ("freeing settings");
    for (size_t si = 0; si < SF_count; si++) {
        delete settings[si];
        settings[si] = NULL;
    }
}

void Settings::load () {
    logger.fineln ("loading settings");
    for (size_t si = 0; si < SF_count; si++) {
        settings[si]->load ();
    }
}

void Settings::save () {
    logger.fineln ("saving settings");
    for (size_t si = 0; si < SF_count; si++) {
        settings[si]->save ();
    }
}

