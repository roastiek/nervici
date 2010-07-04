/* 
 * File:   utils.h
 * Author: bobo
 *
 * Created on 2. červenec 2010, 13:00
 */

#ifndef UTILS_H
#define	UTILS_H

#include <sys/types.h>

typedef char * String;
typedef const char * UnownedString;

/*
 * produce excat copy of source
 * must be freed
 */
String str_copy (UnownedString source);
/*
 * produce begin + end
 * must be freed
 */
String str_concat (UnownedString begin, UnownedString end);
/*
 * produce source, but result has allocated space bytes more then needed
 * must be freed
 */
String str_bigger_copy (UnownedString source, size_t space);
/*
 * produce path + '/' + file
 * must be freed
 */
String str_concat_path (UnownedString path, UnownedString file);

#define array_append(array,item,t) (array).count++;\
(array).items = (t) realloc ((array).items, sizeof (item) * (array).count);\
(array).items[(array).count - 1] = item

/*typedef struct {
    size_t size;
    char *buffer;
} CharBuffer;*/

#endif	/* UTILS_H */

