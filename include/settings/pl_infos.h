#ifndef __PLINFO_H__
#define __PLINFO_H__

#include <vector>

#include "fakes/pl_info.h"

class PlInfos {
private:

    static std::vector<PlInfo*> players;

    static std::vector<PlInfo*> ais;

    PlInfos ();

    static void load_players ();

    static void load_ais ();

    static void save_players ();

    static void save_ais ();

public:
    static void load ();

    static void save_and_free ();

    static size_t count ();

    static size_t players_count ();

    static size_t ais_count ();

    static PlInfo& at (size_t index);

    static void add (const PlInfo& info);

    static void remove (size_t idi);

};

inline size_t PlInfos::count () {
    return players.size () + ais.size ();
}

inline size_t PlInfos::players_count () {
    return players.size ();
}

inline size_t PlInfos::ais_count () {
    return ais.size ();
}

inline PlInfo & PlInfos::at (size_t index) {
    return (index < players.size ()) ? *players[index] : *ais[index
            - players.size ()];
}

#endif // __PLINFO_H__
