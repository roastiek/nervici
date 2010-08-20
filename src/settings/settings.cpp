/*
 * settings.cpp
 *
 *  Created on: 20.8.2010
 *      Author: bobo
 */

#include "settings/setting.h"
#include "settings/settings.h"

Settings Settings::instance;

Settings& settings = Settings::get_instance();

Settings::Settings () {
    settings.push_back(new Setting("nervici.conf"));
    settings.push_back(new Setting("game.conf"));
    settings.push_back(new Setting("players.conf"));
    settings.push_back(new Setting("plastiks.conf"));
    settings.push_back(new Setting("teams.conf"));
}

Settings::~Settings() {
    for (size_t si = 0; si < settings.size (); si++) {
        delete settings[si];
    }
    settings.clear();
}

Setting& Settings::app () {
    return *settings[0];
}

Setting& Settings::game () {
    return *settings[1];
}

Setting& Settings::players () {
    return *settings[2];
}

Setting& Settings::ais () {
    return *settings[3];
}

Setting& Settings::teams () {
    return *settings[4];
}

void Settings::load () {
    for (size_t si = 0; si < settings.size (); si++) {
        settings[si]->load();
    }
}

void Settings::save () {
    for (size_t si = 0; si < settings.size (); si++) {
        settings[si]->save();
    }
}

Settings& Settings::get_instance() {
    return instance;
}
