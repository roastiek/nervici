#include "settings/pl_info.h"

using namespace Glib;

PlInfo::PlInfo () {
}

PlInfo::PlInfo (uint32_t nc,
        const ustring& nn,
        bool jump,
        int distance,
        const ustring& np,
        int npi) :
    color (nc), type (PT_AI), name (nn), profil (np), pitch (npi) {

    ai.gen = AI_GEN_0;
    ai.gen0.jump = jump;
    ai.gen0.distance = distance;
}

