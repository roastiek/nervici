/*
 * mod_info.h
 *
 *  Created on: 21.8.2010
 *      Author: bobo
 */

#ifndef MOD_INFO_H_
#define MOD_INFO_H_

#include "mods/mod_specification.h"

struct ModInfo {
    const char* name;
    const char* autor;
    const char* rules;
    ModSpecification spec;
};

#endif /* MOD_INFO_H_ */
