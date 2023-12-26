#pragma once

// DESCRIPTION
//      returns true if str is whitespace
bool is_whitespace(const char *str);

// DESCRIPTION
//      returns true if str has at least one occurrence of c
bool has_char(const char *str, const char c);

// DESCRIPTION
//      returns true if str ends with c
bool ends_char(const char *str, const char c);

// DESCRIPTION
//      returns true if str ends with substr
bool ends_substr(const char *str, const char *substr);

// DESCRIPTION
//      returns true if str is an int
bool is_int(const char *str);

// DESCRIPTION
//      returns true if str is a uint
bool is_uint(const char *str);

// DESCRIPTION
//      returns true if str is a float
bool is_float(const char *str);

// DESCRIPTION
//      returns true if str is a double
bool is_double(const char *str);