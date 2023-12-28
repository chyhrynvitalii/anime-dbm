#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anime-dbm-rec.h"
#include "file.h"
#include "get.h"
#include "maths.h"


// length of database record keys
const size_t title_len = 128, status_len = 16, score_len = 4, prog_len = 4, rec_key_len = 8, sort_ord_len = 16;

// domain of score
const float score_low = 0, score_up = 10;

// DESCRIPTION
//      record print and scan csv formats
const char *rec_printf_csv = "\"%s\",\"%s\",%.2f,%u\n";
const char *rec_scanf_csv = "\"%[^\"]\",\"%[^\"]\",%f,%u%*c";

// DESCRIPTION
//     record print toml format
const char *rec_printf_toml = "[%s]\n"
                              "status = %s\n"
                              "score = %.2f\n"
                              "progress = %u\n";

rec *alloc_rec() {
    rec *rec = calloc(1, sizeof(*rec));
    rec->title = calloc(title_len, sizeof(char));
    rec->status = calloc(status_len, sizeof(char));
    return rec;
}

void free_rec(rec *rec) {
    free(rec->title);
    free(rec->status);
    free(rec);
}

rec **alloc_recs(int rec_num) {
    rec **recs = calloc(rec_num, sizeof(rec));
    for (int i = 0; i < rec_num; ++i) {
        recs[i] = alloc_rec();
    }
    return recs;
}

void free_recs(rec **recs, int rec_num) {
    for (int i = 0; i < rec_num; ++i) {
        free_rec(recs[i]);
    }
    free(recs);
}

int get_rec_title(rec *rec) {
    if (get_str(title_len, "title: ", rec->title) == -1) {
        return -1;
    }
    return 0;
}

int get_rec_status(rec *rec) {
    if (get_str(status_len, "status: ", rec->status) == -1) {
        return -1;
    }
    return 0;
}

int get_rec_score(rec *rec) {
    if (get_float(score_len, "score: ", &rec->score) == -1) {
        return -1;
    } else if (!in_closed_inter(rec->score, score_low, score_up)) {
        errno = EDOM;
        return -1;
    }
    return 0;
}

int get_rec_prog(rec *rec) {
    if (get_uint(prog_len, "progress: ", &rec->prog) == -1) {
        return -1;
    }
    return 0;
}

int get_rec(rec *rec) {
    if (get_rec_title(rec) == -1) {
        return -1;
    }
    if (get_rec_status(rec) == -1) {
        return -1;
    }
    if (get_rec_score(rec) == -1) {
        return -1;
    }
    if (get_rec_prog(rec) == -1) {
        return -1;
    }
    return 0;
}

int append_rec(const char *db_name, rec *rec) {
    FILE *db = fopen(db_name, "a");
    if (db == NULL) {
        return -1;
    } else {
        fprintf(db, rec_printf_csv, rec->title, rec->status, rec->score, rec->prog);
        fclose(db);
        return 0;
    }
}

int write_recs(const char *db_name, rec **recs, int rec_num) {
    FILE *db = fopen(db_name, "w");
    if (db == NULL) {
        return -1;
    }

    for (int i = 0; i < rec_num; ++i) {
        if (recs[i] != NULL) {
            append_rec(db_name, recs[i]);
        }
    }
    return 0;
}

int scan_recs(char *db_name, rec **recs, int rec_num) {
    FILE *db = fopen(db_name, "r");
    if (db == NULL) {
        return -1;
    }
    for (int i = 0; i < rec_num; ++i) {
        fscanf(db, rec_scanf_csv, recs[i]->title, recs[i]->status, &recs[i]->score, &recs[i]->prog);
    }
    fclose(db);
    return 0;
}

void ls_rec_titles(rec **recs, int rec_num) {
    for (int i = 0; i < rec_num; ++i) {
        puts(recs[i]->title);
    }
}

int get_rec_title_str(char *rec_title_buf) {
    if (get_str(title_len, "title: ", rec_title_buf) == -1) {
        return -1;
    } else {
        return 0;
    }
}

rec *get_match_title_rec(rec **recs, const char *rec_title, int rec_num) {
    for (int i = 0; i < rec_num; ++i) {
        if (strcmp(recs[i]->title, rec_title) == 0) {
            return recs[i];
        }
    }
    return NULL;
}

rec *get_target_rec(rec **recs, int rec_num) {
    ls_rec_titles(recs, rec_num);
    char *rec_title = calloc(title_len, sizeof(char));
    if (get_rec_title_str(rec_title) == -1) {
        free(rec_title);
        return NULL;
    }

    rec *target_rec = get_match_title_rec(recs, rec_title, rec_num);
    if (target_rec == NULL) {
        free(rec_title);
        errno = EINVAL;
        return NULL;
    } else {
        return target_rec;
    }
}

void put_rec(rec *rec) {
    printf(rec_printf_toml, rec->title, rec->status, rec->score, rec->prog);
}

enum rec_key get_rec_key() {
    char *rec_key = calloc(title_len, sizeof(char));
    if (get_str(rec_key_len, "record key: ", rec_key) == -1) {
        return -1;
    }
    if (strcasecmp(rec_key, "title") == 0) {
        free(rec_key);
        return TITLE;
    } else if (strcasecmp(rec_key, "status") == 0) {
        free(rec_key);
        return STATUS;
    } else if (strcasecmp(rec_key, "score") == 0) {
        free(rec_key);
        return SCORE;
    } else if (strcasecmp(rec_key, "progress") == 0) {
        free(rec_key);
        return PROG;
    } else {
        free(rec_key);
        errno = EINVAL;
        return NO_REC_KEY;
    }
}

int edit_rec_key(rec *rec, enum rec_key rec_key) {
    switch (rec_key) {
        case TITLE: {
            get_rec_title(rec);
            return 0;
        }
        case STATUS: {
            get_rec_status(rec);
            return 0;
        }
        case SCORE: {
            get_rec_score(rec);
            return 0;
        }
        case PROG: {
            get_rec_prog(rec);
            return 0;
        }
        default:
            return -1;
    }
}

enum sort_ord get_sort_ord() {
    char *sort_ord = calloc(sort_ord_len, sizeof(char));
    if (get_str(sort_ord_len, "sorting order (ascending or descending): ", sort_ord) == -1) {
        return -1;
    }
    if (strcasecmp(sort_ord, "ascending") == 0) {
        free(sort_ord);
        return ASC;
    } else if (strcasecmp(sort_ord, "descending") == 0) {
        free(sort_ord);
        return DESC;
    } else {
        free(sort_ord);
        errno = EINVAL;
        return NO_SORT_ORD;
    }
}

int new_rec(char *db_name) {
    rec *rec = alloc_rec();

    if (get_rec(rec) == -1) {
        free_rec(rec);
        return -1;
    }

    if (append_rec(db_name, rec) == -1) {
        return -1;
    } else {
        printf("record %s has been created\n", rec->title);
        free_rec(rec);
        return 0;
    }
}

int read_rec(char *db_name) {
    int rec_num = get_rec_num_csv(db_name);
    if (rec_num == -1) {
        return -1;
    } else if (rec_num == 0) {
        puts("there are no records in the database");
        return 0;
    }

    rec **recs = alloc_recs(rec_num);
    if (scan_recs(db_name, recs, rec_num) == -1) {
        free_recs(recs, rec_num);
        return -1;
    }

    rec *target_rec = get_target_rec(recs, rec_num);
    if (target_rec == NULL) {
        free_recs(recs, rec_num);
        return -1;
    } else {
        put_rec(target_rec);
        free_recs(recs, rec_num);
        return 0;
    }
}

int edit_rec(char *db_name) {
    int rec_num = get_rec_num_csv(db_name);
    if (rec_num == -1) {
        return -1;
    } else if (rec_num == 0) {
        puts("there are no records in the database");
        return 0;
    }

    rec **recs = alloc_recs(rec_num);
    if (scan_recs(db_name, recs, rec_num) == -1) {
        free_recs(recs, rec_num);
        return -1;
    }

    rec *target_rec = get_target_rec(recs, rec_num);
    if (target_rec == NULL) {
        free_recs(recs, rec_num);
        return -1;
    }

    enum rec_key rec_key = get_rec_key();
    if (rec_key == NO_REC_KEY) {
        free_recs(recs, rec_num);
        return -1;
    }

    if (edit_rec_key(target_rec, rec_key) == -1) {
        free_recs(recs, rec_num);
        return -1;
    }

    if (write_recs(db_name, recs, rec_num) == -1) {
        free_recs(recs, rec_num);
        return -1;
    } else {
        printf("the record %s has been edited\n", target_rec->title);
        free_recs(recs, rec_num);
        return 0;
    }
}

int del_rec(char *db_name) {
    int rec_num = get_rec_num_csv(db_name);
    if (rec_num == -1) {
        return -1;
    } else if (rec_num == 0) {
        puts("there are no records in the database");
        return 0;
    }

    rec **recs = alloc_recs(rec_num);
    if (scan_recs(db_name, recs, rec_num) == -1) {
        free_recs(recs, rec_num);
        return -1;
    }

    rec *target_rec = get_target_rec(recs, rec_num);
    if (target_rec == NULL) {
        free_recs(recs, rec_num);
        return -1;
    }

    for (int i = 0; i < rec_num; i++) {
        if (target_rec == recs[i]) {
            free_rec(recs[i]);
            recs[i] = NULL;
        }
    }

    if (write_recs(db_name, recs, rec_num) == -1) {
        free_recs(recs, rec_num);
        return -1;
    } else {
        puts("the record has been deleted");
        free_recs(recs, rec_num);
        return 0;
    }
}