#ifndef __PLINFO_H__
#define __PLINFO_H__

#include <vector>

#include "int_type.h"
#include "fakes/pl_info.h"

class PlInfos {
private:
    
    std::vector<PlInfo> players;
    
    std::vector<PlInfo> ais;
    
    static PlInfos instance;
    
    PlInfos ();
    
    void load_players ();

    void load_ais ();
    
    void save_players ();
    
    void save_ais ();

public:
    void load ();

    void save ();

    size_t count ();

    size_t players_count ();

    size_t ais_count ();

    const PlInfo& operator[] (size_t index) const;
    
    PlInfo& operator[] (size_t index);

    void add (const PlInfo& info);

    void remove (size_t idi);
    
    static PlInfos& get_instance ();
};

extern PlInfos& pl_infos;

#endif // __PLINFO_H__
