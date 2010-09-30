#ifndef __PLINFO_H__
#define __PLINFO_H__

#include <vector>

#include "fakes/ai_info.h"
#include "fakes/pl_info.h"
#include "fakes/glibmm_decl.h"

class PlInfos {
private:

    std::vector<PlInfo*> players;

    std::vector<PlInfo*> ais;

    static PlInfos instance;
    
    PlInfos ();
    
    ~PlInfos ();

    void load_players ();

    void load_ais ();

    void save_players ();

    void save_ais ();
    
    void load_ai (AIInfo& info, const Glib::ustring& section);
    
    void save_ai (const AIInfo& info, const Glib::ustring& section);

public:
    void load ();

    void save ();

    size_t count () const;

    size_t players_count () const;

    size_t ais_count () const;

    const PlInfo& operator[] (size_t index) const;

    PlInfo& operator[] (size_t index);

    void add (const PlInfo& info);

    void remove (size_t idi);
    
    static PlInfos& get_instance ();
};

extern PlInfos& pl_infos;

inline size_t PlInfos::count () const {
    return players.size () + ais.size ();
}

inline size_t PlInfos::players_count () const{
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

inline PlInfos& PlInfos::get_instance() {
    return instance;
}

#endif // __PLINFO_H__
