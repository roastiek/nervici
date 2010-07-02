/* 
 * File:   utils.h
 * Author: bobo
 *
 * Created on 2. červenec 2010, 13:00
 */

#ifndef UTILS_H
#define	UTILS_H

#include <sys/types.h>

#ifdef	__cplusplus
extern "C" {
#endif

char *str_copy (const char *source);

char *str_concat (const char *begin, const char *end);
/*
 * produce source, but result has allocated space bytes more then needed
 */
char *str_bigger_copy (const char *source, size_t space);
/*
 * produce path + '/' + file
 */
char *str_concat_path (const char *path, const char *file);


#define array_append(array,item) (array).count++;\
(array).items = realloc ((array).items, sizeof (item) * (array).count);\
(array).items[(array).count - 1] = item

typedef struct {
    size_t size;
    char *buffer;
} CharBuffer;

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

