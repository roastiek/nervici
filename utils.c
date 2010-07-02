#include <string.h>
#include <stdlib.h>

#include "utils.h"

char *str_copy (const char *source) {
    size_t len;
    char *result;

    len = strlen (source);
    result = malloc (len + 1);
    memcpy(result, source, len + 1);

    return result;
}

char *str_concat (const char *begin, const char *end) {
    size_t begin_len;
    size_t end_len;
    char *result;

    begin_len = strlen(begin);
    end_len = strlen (end);

    result = malloc (begin_len + end_len + 1);
    memcpy(result, begin, begin_len);
    memcpy(result + begin_len, end, end_len + 1);

    return result;
}

char *str_bigger_copy (const char *source, size_t space) {
    size_t len;
    char *result;

    len = strlen (source);
    result = malloc (len + 1 + space);
    memcpy(result, source, len + 1);

    return result;
}

char *str_concat_path (const char *path, const char *file) {
    size_t path_len;
    size_t file_len;
    char *result;

    path_len = strlen(path);
    file_len = strlen (file);

    result = malloc (path_len + 1 + file_len + 1);
    memcpy(result, path, path_len);
    result[path_len] = '/';
    memcpy(result + path_len + 1, file, file_len + 1);

    return result;
}
