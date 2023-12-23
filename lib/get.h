#pragma once

#define buf_pad(buf_len)    ((buf_len) + 2)

void clr_stdin();

int get_int(size_t, const char *, int *);
int get_uint(size_t, const char *, unsigned *);
int get_float(size_t, const char *, float *);
int get_double(size_t, const char *, double *);
int get_str(size_t, const char *, char *);