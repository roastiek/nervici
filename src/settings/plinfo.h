#ifndef __PLINFO_H__
#define __PLINFO_H__

#include <string>
#include <sys/types.h>

using namespace std;

#include "plinfo_defs.h"

struct PlInfos {
private:

public:
    static void load ();

    static void save ();

    size_t get_count ();

    const PlInfo& get (size_t idi);
};


#endif // __PLINFO_H__
