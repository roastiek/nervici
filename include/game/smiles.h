/* 
 * File:   smiles.h
 * Author: bobo
 *
 * Created on 23. červenec 2010, 15:45
 */

#ifndef SMILES_H
#define	SMILES_H

#include "game_info.h"


namespace Smiles {
    void initialize (const GameInfo& info);

    void uninitialize ();

    void step ();

    void erase ();

    void update ();

    void drop (smileid_tu sid);
};

#endif	/* SMILES_H */

