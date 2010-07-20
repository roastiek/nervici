#ifndef __PLINFO_H__
#define __PLINFO_H__

#include <sys/types.h>
#include <vector>

using namespace std;

#include "plinfo_defs.h"

struct PlInfos {
private:
    static PlInfo ais[AIS_COUNT];
    static vector<PlInfo> players;

public:
    static void load ();

    static void save ();

    static size_t get_count ();

    static const PlInfo& get (size_t idi);
};


#endif // __PLINFO_H__
