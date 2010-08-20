/* 
 * File:   smiles.h
 * Author: bobo
 *
 * Created on 23. červenec 2010, 15:45
 */

#ifndef SMILES_H
#define	SMILES_H

#include <vector>

#include "fakes/game_info.h"
#include "fakes/smile.h"

class Smiles {
private:
    std::vector<Smile*> smiles;

    static Smiles instance;

    Smiles ();

public:
    void initialize (const GameInfo& info);

    void uninitialize ();

    void step ();

    void erase ();

    smileid_tu count () const;

    Smile& operator[] (smileid_tu index);

    const Smile& operator[] (smileid_tu index) const;

    static Smiles& get_instance ();
};

extern Smiles& smiles;

#endif	/* SMILES_H */

