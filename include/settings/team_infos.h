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

    static std::vector<TeamInfo*> infos;

    TeamInfos ();

public:
    static void load ();

    static void save_and_free ();

    static size_t count ();

    static TeamInfo& at (size_t index);
};

inline size_t TeamInfos::count () {
    return infos.size ();
}

inline TeamInfo& TeamInfos::at (size_t id) {
    return *infos[id];
}


#endif	/* TEAM_INFOS_H */

