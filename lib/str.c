#include <stdlib.h>
#include <string.h>

#include "str.h"

// parse strings

bool is_whitespace(char *str) {
	return strspn(str, " \t\n\v\r") == strlen(str);
}

bool has_char(char *str, const char c) {
    return strchr(str, c) != NULL;
}

bool ends_w_char(char *str, const char c) {
    return strrchr(str, c) == str + strlen(str) - 1;
}

bool ends_w_substr(char *str, const char *substr) {
    char *endstr = str;
    while ((endstr = strstr(endstr, substr)) != NULL) {
        if (endstr[strlen(substr)] == '\0') {
            return true;
        }
        endstr += strlen(substr);
    }
    return false;
}

// parse strings as values

bool is_int(char *str) {
	char *end_ptr;
	strtol(str, &end_ptr, 10);
	return end_ptr != str && is_whitespace(end_ptr);
}

bool is_uint(char *str) {
    char *end_ptr;
    strtoul(str, &end_ptr, 10);
    return !has_char(str, '-') && end_ptr != str && is_whitespace(end_ptr);
}

bool is_float(char *str) {
    char *end_ptr;
    strtof(str, &end_ptr);
    return end_ptr != str && is_whitespace(end_ptr);
}

bool is_double(char *str) {
    char *end_ptr;
    strtod(str, &end_ptr);
    return end_ptr != str && is_whitespace(end_ptr);
}