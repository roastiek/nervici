#include <cstdio>

#include "utils.h"

#define BUFF_LEN 32

const char* int_to_string (long int value) {
    static char buff[BUFF_LEN];
    snprintf (buff, BUFF_LEN, "%li", value);
    return buff;
}

const char* uint_to_string (long unsigned int value) {
    static char buff[BUFF_LEN];
    snprintf (buff, BUFF_LEN, "%lu", value);
    return buff;
}

const char* double_to_string (double value) {
    static char buff[BUFF_LEN];
    snprintf (buff, BUFF_LEN, "%e", value);
    return buff;
}
