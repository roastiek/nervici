/*
 * logger.cpp
 *
 *  Created on: 21.9.2010
 *      Author: bobo
 */

#include <cstdio>
#include <stdarg.h>

#include "logger.h"

Logger::Logger () {

}

//#define SHOW_FINE
#define SHOW_DEBUG

void Logger::fineln (const char* str, ...) {
#ifdef SHOW_FINE
    va_list arg;

    va_start (arg, str);
    vprintf (str, arg);
    va_end (arg);
    printf ("\n");
#endif 
}

void Logger::debugln (const char* str, ...) {
#ifdef SHOW_DEBUG
    va_list arg;

    va_start (arg, str);
    vprintf (str, arg);
    va_end (arg);
    printf ("\n");
#endif 
}

void Logger::warnln (const char* str, ...) {
    va_list arg;

    va_start (arg, str);
    vprintf (str, arg);
    va_end (arg);
    printf ("\n");
}

void Logger::errln (const char* str, ...) {
    va_list arg;

    va_start (arg, str);
    vfprintf (stderr, str, arg);
    va_end (arg);
    fprintf (stderr, "\n");
}

Logger Logger::instance;

Logger& logger = Logger::get_instance ();
