/* 
 * File:   smyle_types.h
 * Author: bobo
 *
 * Created on 23. červenec 2010, 1:49
 */

#ifndef SMYLE_TYPES_H
#define	SMYLE_TYPES_H

enum SmileType {
    ST_pozi, ST_nega, ST_fleg, ST_iron, ST_ham, ST_dest, ST_term, ST_count
};

inline SmileType operator ++(SmileType& st, int) {
    st = (SmileType) (st + 1);
    return st;
}


#endif	/* SMYLE_TYPES_H */

