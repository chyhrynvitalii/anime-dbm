#pragma once

// DESCRIPTION
//      adds padding of two bytes to buf_len to facilitate newline and null terminator
#define buf_pad(buf_len)    ((buf_len) + 2)

// DESCRIPTION
//      clear standard input
void clr_stdin();

// DESCRIPTION
//      prints out get_msg in case it's not NULL
//      gets an int from stdin (reads up to buf_pad(buf_len) characters)
//      stores it in int_buf
// RETURN VALUES
//      returns -1 on error, 0 on success
// ERRORS
//      sets errno to EIO if input exceeds buf_len
//      sets EINVAL if input is not an int
int get_int(size_t buf_len, const char *get_msg, int *int_buf);

// DESCRIPTION
//      prints out get_msg in case it's not NULL
//      gets a uint from stdin (reads up to buf_pad(buf_len) characters)
//      stores it in uint_buf
// RETURN VALUES
//      returns -1 on error, 0 on success
// ERRORS
//      sets errno to EIO if input exceeds buf_len
//      sets errno to EINVAL if input is not a uint
int get_uint(size_t buf_len, const char *get_msg, unsigned *uint_buf);

// DESCRIPTION
//      prints out get_msg in case it's not NULL
//      gets a float from stdin (reads up to buf_pad(buf_len) characters)
//      stores it in float_buf
// RETURN VALUES
//      returns -1 on error, 0 on success
// ERRORS
//      sets errno to EIO if input exceeds buf_len
//      sets errno to EINVAL if input is not a float
int get_float(size_t buf_len, const char *get_msg, float *float_buf);

// DESCRIPTION
//      prints out get_msg in case it's not NULL
//      gets a double from stdin (reads up to buf_pad(buf_len) characters)
//      stores it in double_buf
// RETURN VALUES
//      returns -1 on error, 0 on success
// ERRORS
//      sets errno to EIO if input exceeds buf_len
//      sets errno to EINVAL if input is not a double
int get_double(size_t buf_len, const char *get_msg, double *double_buf);

// DESCRIPTION
//      prints out get_msg in case it's not NULL
//      gets a str from stdin (reads up to buf_pad(buf_len) characters)
//      stores it in str_buf
// RETURN VALUES
//      returns -1 on error, 0 on success
// ERRORS
//      sets errno to EIO if input exceeds buf_len
int get_str(size_t buf_len, const char *get_msg, char *str_buf);