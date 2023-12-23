#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "anime-db-engine.h"
#include "get.h"
#include "maths.h"
#include "str.h"

const char *dbfext= ".csv";

/*
ent get_ent() {
    ent ent;
    ent.title = calloc(title_len, sizeof(char));
    ent.status = calloc(status_len, sizeof(char));

    do {
        errno = 0;
        if (get_str(title_len, "title: ", ent.title) == -1) {
            perror("error");
        }
    } while (errno != 0);

    do {
        errno = 0;
        if (get_str(status_len, "status: ", ent.status) == -1) {
            perror("error");
        }
    } while (errno != 0);

    do {
        errno = 0;
        if (get_float(score_len, "score: ", &ent.score) == -1) {
            perror("error");
        } else if (!in_closed_range(ent.score, score_low, score_up)) {
            errno = EDOM;
            perror("error");
        }
    } while (errno != 0);

    do {
        errno = 0;
        if (get_uint(prog_len, "progress: ", &ent.prog) == -1) {
            perror("error");
        }
    } while (errno != 0);

    return ent;
}
*/

/*
void free_ent(ent ent) {
    free(ent.title);
    free(ent.status);
}
*/

// new functions
int get_dbname(char *dbname_buf) {
    errno = 0;
    printf("enter database name (must be a %s file): ", dbfext);
    if (get_str(FILENAME_MAX, NULL, dbname_buf) == -1) {
        return -1;
    } else if (!ends_w_substr(dbname_buf, dbfext)) {
        errno = EINVAL;
        return -1;
    }
    return 0;
}