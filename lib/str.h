#pragma once

// returns true if a string has a character, otherwise returns false
bool has_char(char *, char);

// returns true if a string ends with a character, otherwise returns false
bool ends_char(char *, char);

// returns true if a string ends with a substring, otherwise returns false
bool ends_substr(char *, const char *);

// returns true if string is an integer, otherwise returns false
bool is_int(char *);

// returns true if string is an unsigned integer, otherwise returns false
bool is_uint(char *);

// returns true if string is a floating point number, otherwise returns false
bool is_float(char *);

// returns true if string is a double precision floating point number, otherwise returns false
bool is_double(char *);