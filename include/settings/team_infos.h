/* 
 * File:   team_infos.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 15:40
 */

#ifndef TEAM_INFOS_H
#define	TEAM_INFOS_H

#include <vector>

#include "fakes/team_info.h"

class TeamInfos {
private:

    std::vector<TeamInfo> infos;

    static TeamInfos instance;

    TeamInfos ();

public:
    size_t count () const;

    TeamInfo& operator[] (size_t index);

    const TeamInfo& operator[] (size_t index) const;

    void load ();

    void save ();

    static TeamInfos& get_instance ();
};

extern TeamInfos& team_infos;

#endif	/* TEAM_INFOS_H */

