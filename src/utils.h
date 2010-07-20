/* 
 * File:   utils.h
 * Author: bobo
 *
 * Created on 20. červenec 2010, 23:21
 */

#ifndef UTILS_H
#define	UTILS_H

#include <sstream>

using namespace std;

template <class T>
inline string to_string (const T& value) {
    stringstream ss;
    ss << value;
    return ss.str ();
}

#endif	/* UTILS_H */

