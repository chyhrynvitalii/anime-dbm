#pragma once

// adds padding of two bytes to buf_len to facilitate newline and null terminator
#define buf_pad(buf_len)    ((buf_len) + 2)

// clear standard input
void clr_stdin();

// get an integer, store it in a pointer
// returns -1 on error, 0 on success
int get_int(size_t, const char *, int *);

// get an unsigned integer, store it in a pointer
// returns -1 on error, 0 on success
int get_uint(size_t, const char *, unsigned *);

// get a floating-point number, store it in a pointer
// returns -1 on error, 0 on success
int get_float(size_t, const char *, float *);

// get a double-precision floating-point number, store it in a pointer
// returns -1 on error, 0 on success
int get_double(size_t, const char *, double *);

// get a string, store it in a pointer
// returns -1 on error, 0 on success
int get_str(size_t, const char *, char *);