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

    std::vector<TeamInfo*> infos;

    static TeamInfos instance;
    
    TeamInfos ();
    
    ~TeamInfos ();

public:
    void load ();

    void save ();

    size_t count () const;

    const TeamInfo& operator[] (size_t index) const;

    TeamInfo& operator[] (size_t index);
    
    static TeamInfos& get_instance ();
};

extern TeamInfos& team_infos;

inline size_t TeamInfos::count () const {
    return infos.size ();
}

inline const TeamInfo& TeamInfos::operator [] (size_t id) const {
    return *infos[id];
}

inline TeamInfo& TeamInfos::operator [] (size_t id) {
    return *infos[id];
}

inline TeamInfos& TeamInfos::get_instance() {
    return instance;
}


#endif	/* TEAM_INFOS_H */

