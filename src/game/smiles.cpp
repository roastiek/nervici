#include <vector>

#include "logger.h"
#include "engine/render.h"
#include "game/smile.h"
#include "game/game_info.h"

#include "game/smiles.h"

using namespace std;

Smiles Smiles::instance;

Smiles& smiles = Smiles::get_instance ();

Smiles::Smiles () {
}

bool Smiles::initialize (const SmileSetting& info) {
    logger.fineln ("initialize smiles");

    smileid_tu order = 0;

    for (int sti = ST_pozi; sti < ST_count; sti++) {
        for (int li = 0; li < 3; li++) {
            for (int ci = 0; ci < info.counts[sti][li]; ci++) {
                smiles.push_back (SmileFactory::create (smiles.size (), order,
                        info.counts[sti][li], SmileType (sti), li + 1));
                order++;
            }
            order = 0;
        }
    }

    if (!render.load_smiles (info))
        return false;

    return true;
}

void Smiles::uninitialize () {
    logger.fineln ("freeing smiles");

    render.free_smiles ();

    for (size_t si = 0; si < smiles.size (); si++) {
        delete smiles[si];
    }

    smiles.clear ();
}

void Smiles::step () {
    for (size_t si = 0; si < smiles.size (); si++) {
        smiles[si]->step ();
    }
}

void Smiles::erase () {
    for (size_t si = 0; si < smiles.size (); si++) {
        smiles[si]->erase ();
    }
}

smileid_tu Smiles::count () const {
    return smiles.size ();
}

Smile& Smiles::operator [] (smileid_tu index) {
    return *smiles[index];
}

const Smile& Smiles::operator [] (smileid_tu index) const {
    return *smiles[index];
}

Smiles& Smiles::get_instance () {
    return instance;
}
