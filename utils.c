#include <string.h>
#include <stdlib.h>

#include "utils.h"

String str_copy (UnownedString source) {
    size_t len;
    String result;

    len = strlen (source);
    result = malloc (len + 1);
    memcpy(result, source, len + 1);

    return result;
}

String str_concat (UnownedString begin, UnownedString end) {
    size_t begin_len;
    size_t end_len;
    String result;

    begin_len = strlen(begin);
    end_len = strlen (end);

    result = malloc (begin_len + end_len + 1);
    memcpy(result, begin, begin_len);
    memcpy(result + begin_len, end, end_len + 1);

    return result;
}

String str_bigger_copy (UnownedString source, size_t space) {
    size_t len;
    String result;

    len = strlen (source);
    result = malloc (len + 1 + space);
    memcpy(result, source, len + 1);

    return result;
}

String str_concat_path (UnownedString path, UnownedString file) {
    size_t path_len;
    size_t file_len;
    String result;

    path_len = strlen(path);
    file_len = strlen (file);

    result = malloc (path_len + 1 + file_len + 1);
    memcpy(result, path, path_len);
    result[path_len] = '/';
    memcpy(result + path_len + 1, file, file_len + 1);

    return result;
}
