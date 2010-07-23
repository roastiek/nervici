#include <vector>
using namespace std;

#include "game/smile.h"
#include "engine/render.h"

#include "game/smiles.h"

namespace Smiles {

static vector<Smile*> smiles;

void initialize (const GameInfo& info) {
    for (SmileType sti = ST_pozi; sti < ST_count; sti++) {
        for (int li = 0; li < 3; li++) {
            for (int ci = 0; ci < info.smiles.counts[sti][li]; ci++) {
                Smile* sm = SmileFactory::create (smiles.size (), sti, li + 1);
                smiles.push_back (sm);
            }
        }
    }

    Render::load_smiles (info);
}

void uninitialize () {
    Render::free_smiles ();

    for (size_t si = 0; si < smiles.size (); si++) {
        delete smiles[si];
    }

    smiles.clear ();
}

void step () {
    for (size_t si = 0; si < smiles.size (); si++) {
        smiles[si]->step ();
    }
}

void erase () {
    for (size_t si = 0; si < smiles.size (); si++) {
        smiles[si]->erase ();
    }
}

void update () {
    for (size_t si = 0; si < smiles.size (); si++) {
        smiles[si]->update ();
    }
}

void drop (smileid_tu sid) {
    smiles[sid]->drop ();
}

}