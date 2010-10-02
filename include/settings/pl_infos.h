/*
 * Stores informations about all avaiable players for a game.  
 */

#ifndef __PLINFO_H__
#define __PLINFO_H__

#include <vector>

#include "fakes/ai_info.h"
#include "fakes/pl_info.h"
#include "fakes/glibmm_decl.h"

/*
 * Using singleton pattern
 * PLInfos class is a simple colletion of PlInfo objects. It supports accessing,
 * adding, removing and pernament storing on a disk.  
 */

class PlInfos {
private:

    std::vector<PlInfo*> players;

    std::vector<PlInfo*> ais;

    static PlInfos instance;

    PlInfos ();

    ~PlInfos ();

    /*
     * Loads human players.
     */
    void load_players ();

    /*
     * Loads all AI players or create default's ones if none was found.
     */
    void load_ais ();

    /*
     * Saves human players.
     */
    void save_players ();

    /*
     * Saves AI players
     */
    void save_ais ();

    /*
     * Loads one AI player
     */
    void load_ai (AIInfo& info, const Glib::ustring& section);

    /*
     * Stores one AI player. Different AIs are saveing different informations.
     */
    void save_ai (const AIInfo& info, const Glib::ustring& section);

public:
    void load ();

    void save ();

    /*
     * Returns count of all players, humans and AIs together.
     */
    size_t count () const;

    size_t players_count () const;

    size_t ais_count () const;

    /*
     * Accesing one player. Humans are indexed in an interval between 0 and
     * players_count - 1, AIs between players_count and players_count + 
     * ais_count - 1. 
     */
    PlInfo& operator[] (size_t index);
    const PlInfo& operator[] (size_t index) const;

    void add (const PlInfo& info);

    /*
     * Remove a player with specifed index from the colletion. It is using same
     * indexing as accessing. 
     */
    void remove (size_t idi);

    static PlInfos& get_instance ();
};

extern PlInfos& pl_infos;

inline size_t PlInfos::count () const {
    return players.size () + ais.size ();
}

inline size_t PlInfos::players_count () const {
    return players.size ();
}

inline size_t PlInfos::ais_count () const {
    return ais.size ();
}

inline PlInfo & PlInfos::operator [] (size_t index) {
    return (index < players.size ()) ? *players[index] : *ais[index
            - players.size ()];
}

inline const PlInfo & PlInfos::operator [] (size_t index) const {
    return (index < players.size ()) ? *players[index] : *ais[index
            - players.size ()];
}

inline PlInfos& PlInfos::get_instance () {
    return instance;
}

#endif // __PLINFO_H__
