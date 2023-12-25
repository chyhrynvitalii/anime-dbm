#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "anime-dbm-engine.h"
#include "get.h"
#include "maths.h"
#include "str.h"

// database file extension
const char *csvfext= ".csv";

// length of entry members
const size_t title_len = 128, status_len = 16, score_len = 4, prog_len = 4;

// domain of score
const float score_low = 0, score_up = 10;

// entry print / scan formats
const char *ent_printf_csv = "\"%s\",\"%s\",%.2f,%u\n";
const char *ent_printf_toml = "[%s]\n"
                              "status = \"%s\"\n"
                              "score = %.2f\n"
                              "progress = %u\n";
const char *ent_scanf_csv = "\"%[^\"]\",\"%[^\"]\",%f,%u%*c";
const char *title_printf_toml = "[%s]\n";

// allocates memory for an entry
// returns NULL on error, pointer to an allocated entry on success
ent *alloc_ent() {
    ent *ent = calloc(1, sizeof(*ent));
    if ((ent->title = calloc(title_len, sizeof(char))) == NULL) {
        return NULL;
    }
    if ((ent->status = calloc(status_len, sizeof(char))) == NULL) {
        return NULL;
    }
    return ent;
}

// deallocates memory pointed by an entry pointer
void free_ent(ent *ent) {
    free(ent->title);
    free(ent->status);
}

// get an entry, store it in a pointer
// returns -1 on error, 0 on success
int get_ent(ent *ent) {
    if (get_str(title_len, "title: ", ent->title) == -1) {
        return -1;
    }
    if (get_str(status_len, "status: ", ent->status) == -1) {
        return -1;
    }
    if (get_float(score_len, "score: ", &ent->score) == -1) {
        return -1;
    } else if (!in_closed_range(ent->score, score_low, score_up)) {
        errno = EDOM;
        return -1;
    }
    if (get_uint(prog_len, "progress: ", &ent->prog) == -1) {
        return -1;
    }

    return 0;
}

// get a database name
// returns -1 on error, 0 on success
int get_dbname(char *dbname_buf) {
    printf("enter database name (must be a %s file): ", csvfext);
    if (get_str(FILENAME_MAX, NULL, dbname_buf) == -1) {
        return -1;
    } else if (!ends_w_substr(dbname_buf, csvfext)) {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

// appends an entry to a database
// returns -1 on error, 0 on success
int append_ent(char *dbname, ent *ent) {
    FILE *db = fopen(dbname, "a");
    if (db == NULL) {
        return -1;
    }
    if (fprintf(db, ent_printf_csv, ent->title, ent->status, ent->score, ent->prog) < 0) {
        return -1;
    }
    if (fclose(db) == EOF) {
        return -1;
    }
    return 0;
}

// calculates a number of entries in a database
// returns -1 on error, number of entries in database on success
int get_ent_num(char *dbname) {
    int ent_num = 0;
    FILE *db = fopen(dbname, "r");
    if (db == NULL) {
        return -1;
    }
    while (feof(db) == 0) {
        if (getc(db) == '\n') {
            ent_num++;
        }
    }
    return ent_num;
}

// scans a database for a specified number of entries, store result in an entry pointer
// returns -1 on error, 0 on success
int scan_db(char *dbname, const char *scan_format, ent **ent, int ent_num) {
    FILE *db = fopen(dbname, "r");
    if (db == NULL) {
        return -1;
    }
    for (int i = 0; i < ent_num; ++i) {
        fscanf(db, scan_format, ent[i]->title, ent[i]->status, &ent[i]->score, &ent[i]->prog);
    }
    fclose(db);
    return 0;
}

// prints out an entry to stdin with a specified format
// returns -1 on error, 0 on success
int printf_ent(ent *ent, const char *print_format) {
    if (printf(print_format, ent->title, ent->status, ent->score, ent->prog) < 0) {
        return -1;
    }
    return 0;
}

// prints out titles of entries to stdin with a specified format
// returns -1 on error, 0 on success
int list_titles(ent **ents, const char *print_format, int ent_num) {
    for (int i = 0; i < ent_num; ++i) {
        if (printf(print_format, ents[i]->title) < 0) {
            return -1;
        }
    }
    return 0;
}

// searches through an array of entry pointers for a matching title
// returns a pointer to an entry with a matching title, NULL if no matches have been found
ent *get_ent_w_match_title(ent **ents, const char *title, int ent_num) {
    for (int i = 0; i < ent_num; ++i) {
        if (strcmp(ents[i]->title, title) == 0) {
            return ents[i];
        }
    }
    return NULL;
}

// erases a database
// returns -1 on error, 0 on success
int erase_db(char *dbname) {
    FILE *db = fopen(dbname, "w");
    if (db == NULL) {
        return -1;
    }
    return 0;
}