#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "get.h"
#include "str.h"

// clear standard input

void clr_stdin() {
    for (char c; (c = getchar()) != '\n';);
}

// get standard data type values
// if an error occurs the functions return -1 and set errno

int get_int(size_t buf_len, const char *get_msg, int *int_buf) {
    char *buf = calloc(buf_pad(buf_len), sizeof(char));

    fputs(get_msg, stdout);
    fgets(buf, buf_pad(buf_len), stdin);

    if (!ends_w_char(buf, '\n')) {
        free(buf);
        errno = EIO;
        clr_stdin();
        return -1;
    } else if (!is_int(buf)) {
        free(buf);
        errno = EINVAL;
        return -1;
    } else {
        *int_buf = (int)strtol(buf, NULL, 10);
        free(buf);
        return 0;
    }
}

int get_uint(size_t buf_len, const char *get_msg, unsigned *uint_buf) {
    char *buf = calloc(buf_pad(buf_len), sizeof(char));

    fputs(get_msg, stdout);
    fgets(buf, buf_pad(buf_len), stdin);

    if (!ends_w_char(buf, '\n')) {
        free(buf);
        errno = EIO;
        clr_stdin();
        return -1;
    } else if (!is_uint(buf)) {
        free(buf);
        errno = EINVAL;
        return -1;
    } else {
        *uint_buf = (unsigned)strtoul(buf, NULL, 10);
        free(buf);
        return 0;
    }
}

int get_float(size_t buf_len, const char *get_msg, float *float_buf) {
    char *buf = calloc(buf_pad(buf_len), sizeof(char));

    fputs(get_msg, stdout);
    fgets(buf, buf_pad(buf_len), stdin);

    if (!ends_w_char(buf, '\n')) {
        free(buf);
        errno = EIO;
        clr_stdin();
        return -1;
    } else if (!is_float(buf)) {
        free(buf);
        errno = EINVAL;
        return -1;
    } else {
        *float_buf = strtof(buf, NULL);
        free(buf);
        return 0;
    }
}

int get_double(size_t buf_len, const char *get_msg, double *double_buf) {
    char *buf = calloc(buf_pad(buf_len), sizeof(char));

    fputs(get_msg, stdout);
    fgets(buf, buf_pad(buf_len), stdin);

    if (!ends_w_char(buf, '\n')) {
        free(buf);
        errno = EIO;
        clr_stdin();
        return -1;
    } else if (!is_double(buf)) {
        free(buf);
        errno = EINVAL;
        return -1;
    } else {
        *double_buf = strtod(buf, NULL);
        free(buf);
        return 0;
    }
}

int get_str(size_t buf_len, const char *get_msg, char *str_buf) {
    fputs(get_msg, stdout);
    fgets(str_buf, buf_pad(buf_len), stdin);

    if (!ends_w_char(str_buf, '\n')) {
        clr_stdin();
        errno = EIO;
        return -1;
    } else {
        str_buf[strlen(str_buf) - 1] = '\0';
        return 0;
    }
}