#include <stdlib.h>
#include <string.h>

#include "str.h"

// parse strings

bool ends_w_newline(char *str) {
	return str[strlen(str) - 1] == '\n' || strcmp(str, "\n") == 0;
}

bool is_whitespace(char *str) {
	return strspn(str, " \t\n\v\r") == strlen(str);
}

bool has_sign(char *str) {
    return strcspn(str, "-+") != strlen(str);
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
    return !has_sign(str) && end_ptr != str && is_whitespace(end_ptr);
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