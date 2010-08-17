/* 
 * File:   statistic.h
 * Author: bobo
 *
 * Created on 17. srpen 2010, 12:17
 */

#ifndef STATISTIC_H
#define	STATISTIC_H

#include "smyle_type.h"

struct Statistic {
public:
    Statistic();

    int deaths;
    int crashes;
    int killed;
    int kills;
    int selfs;
    int jump;
    int smiles[ST_count][4];
};

#endif	/* STATISTIC_H */

