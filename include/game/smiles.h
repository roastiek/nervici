/* 
 * File:   smiles.h
 * Author: bobo
 *
 * Created on 23. červenec 2010, 15:45
 */

#ifndef SMILES_H
#define	SMILES_H

#include "game_info.h"
#include "mods/iplayer.h"

namespace Smiles {
    void initialize (const GameInfo& info);

    void uninitialize ();

    void step ();

    void erase ();

    void update ();

    void eat (smileid_tu sid, plid_tu plid);
}

#endif	/* SMILES_H */

