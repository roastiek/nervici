/* 
 * File:   team_infos.h
 * Author: deedrah
 *
 * Created on 22. červenec 2010, 15:40
 * 
 * Global collection of TeamInfo objects.
 */

#ifndef TEAM_INFOS_H
#define	TEAM_INFOS_H

#include "basic_defs.h"

#include "fakes/team_info.h"

/*
 * Using singleton pattern
 * There are allways TEAMS_COUNT teams. The first one is hidden and used as a 
 * fall back for players without an assigned team.   
 */
class TeamInfos {
private:

    TeamInfo* infos[TEAMS_COUNT];

    static TeamInfos instance;

    TeamInfos ();

    ~TeamInfos ();

public:
    void load ();

    void save ();

    size_t count () const;

    TeamInfo& operator[] (size_t index);
    const TeamInfo& operator[] (size_t index) const;

    static TeamInfos& get_instance ();
};

extern TeamInfos& team_infos;

inline size_t TeamInfos::count () const {
    return TEAMS_COUNT;
}

inline const TeamInfo& TeamInfos::operator [] (size_t id) const {
    return *infos[id];
}

inline TeamInfo& TeamInfos::operator [] (size_t id) {
    return *infos[id];
}

inline TeamInfos& TeamInfos::get_instance () {
    return instance;
}

#endif	/* TEAM_INFOS_H */

