/* 
 * File:   smile_setting.h
 * Author: deedrah
 *
 * Created on 23. červenec 2010, 13:34
 */

#ifndef SMILE_SETTING_H
#define	SMILE_SETTING_H

#include "smile_type.h"
#include "int_type.h"

/*
 * Contains counts of smiles.
 */
struct SmileSetting {
    uint8_t counts[ST_count][3];
};

#endif	/* SMILE_SETTING_H */

