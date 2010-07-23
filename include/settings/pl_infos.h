#ifndef __PLINFO_H__
#define __PLINFO_H__

#include "int_type.h"
#include "pl_info.h"

namespace PlInfos {
    void load ();

    void save ();

    size_t get_count ();

    const PlInfo& get (size_t idi);
};


#endif // __PLINFO_H__