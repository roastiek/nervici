/* 
 * File:   utils.h
 * Author: deedrah
 *
 * Created on 20. červenec 2010, 23:21
 * 
 * Contains some utility functions.
 */

#ifndef UTILS_H
#define	UTILS_H

/*
 * Following functions translate numbers into strings. They return staticly 
 * allocated string, so result is valid only until next call, butt it is not 
 * needed to free him. 
 */
const char* int_to_string (long int value);

const char* uint_to_string (long unsigned int value);

const char* double_to_string (double value);

#endif	/* UTILS_H */

