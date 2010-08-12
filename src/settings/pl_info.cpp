#include "settings/pl_info.h"

using namespace Glib;

PlInfo::PlInfo () {
}

PlInfo::PlInfo (uint32_t nc, const ustring& nn, int nai, const ustring& np, int npi) :
color (nc),
type (PT_AI),
name (nn),
profil (np),
pitch (npi) {
    ai.id = nai;
}

