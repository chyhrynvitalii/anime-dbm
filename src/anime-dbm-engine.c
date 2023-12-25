#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anime-dbm-engine.h"
#include "get.h"
#include "maths.h"
#include "str.h"

// database file extension
const char *csvfext= ".csv";

// length of entry members
const size_t title_len = 128, status_len = 16, score_len = 4, prog_len = 4, ent_member_len = 8, sort_ord_len = 16;

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

// entry member list formats
const char *ent_memb_ls_toml = "[title]\n"
                               "status\n"
                               "score\n"
                               "progress\n";

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

int get_ent_title(ent *ent) {
    if (get_str(title_len, "title: ", ent->title) == -1) {
        return -1;
    }
    return 0;
}

int get_ent_status(ent *ent) {
    if (get_str(status_len, "status: ", ent->status) == -1) {
        return -1;
    }
    return 0;
}

int get_ent_score(ent *ent) {
    if (get_float(score_len, "score: ", &ent->score) == -1) {
        return -1;
    } else if (!in_closed_range(ent->score, score_low, score_up)) {
        errno = EDOM;
        return -1;
    }
    return 0;
}

int get_ent_prog(ent *ent) {
    if (get_uint(prog_len, "progress: ", &ent->prog) == -1) {
        return -1;
    }
    return 0;
}

// get an entry, store it in a pointer
// returns -1 on error, 0 on success
int get_ent(ent *ent) {
    if (get_ent_title(ent) == -1) {
        return -1;
    }
    if (get_ent_status(ent) == -1) {
        return -1;
    }
    if (get_ent_score(ent) == -1) {
        return -1;
    }
    if (get_ent_prog(ent) == -1) {
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
int ls_titles(ent **ents, const char *print_format, int ent_num) {
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

// get sorting order
// returns NO_SORT_ORD and sets errno on error, sorting order on success
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

// get entry member
// returns NO_ENT_MEMB and sets errno on error, entry member on success
enum ent_memb get_ent_memb() {
    char *ent_member_name = calloc(title_len, sizeof(char));
    if (get_str(ent_member_len, "entry member: ", ent_member_name) == -1) {
        return -1;
    }
    if (strcasecmp(ent_member_name, "title") == 0) {
        free(ent_member_name);
        return TITLE;
    } else if (strcasecmp(ent_member_name, "status") == 0) {
        free(ent_member_name);
        return STATUS;
    } else if (strcasecmp(ent_member_name, "score") == 0) {
        free(ent_member_name);
        return SCORE;
    } else if (strcasecmp(ent_member_name, "progress") == 0) {
        free(ent_member_name);
        return PROG;
    } else {
        free(ent_member_name);
        errno = EINVAL;
        return NO_ENT_MEMB;
    }
}

int compar_title_asc(const void *ent1, const void *ent2) {
    ent *ent1_cast = *(ent **)ent1;
    ent *ent2_cast = *(ent **)ent2;
    return strcmp(ent1_cast->title, ent2_cast->title);
}

int compar_status_asc(const void *ent1, const void *ent2) {
    ent *ent1_cast = *(ent **)ent1;
    ent *ent2_cast = *(ent **)ent2;
    return strcmp(ent1_cast->status, ent2_cast->status);
}

int compar_score_asc(const void *ent1, const void *ent2) {
    ent *ent1_cast = *(ent **)ent1;
    ent *ent2_cast = *(ent **)ent2;
    if (ent1_cast->score > ent2_cast->score) {
        return 1;
    } else if (ent1_cast->score < ent2_cast->score) {
        return -1;
    } else {
        return 0;
    }
}

int compar_prog_asc(const void *ent1, const void *ent2) {
    ent *ent1_cast = *(ent **)ent1;
    ent *ent2_cast = *(ent **)ent2;
    return (int)(ent1_cast->prog - ent2_cast->prog);
}

int compar_title_desc(const void *ent1, const void *ent2) {
    return compar_title_asc(ent2, ent1);
}

int compar_status_desc(const void *ent1, const void *ent2) {
    return compar_status_asc(ent2, ent1);
}

int compar_score_desc(const void *ent1, const void *ent2) {
    return compar_score_asc(ent2, ent1);
}

int compar_prog_desc(const void *ent1, const void *ent2) {
    return compar_prog_asc(ent2, ent1);
}

// sorts database
// returns -1 on error, 0 on success
int sort_ents(ent **ents, int ent_num, enum ent_memb ent_memb, enum sort_ord sort_ord) {
    int (*compar_ent_memb)(const void *, const void*);
    switch (ent_memb) {
        case TITLE: {
            switch (sort_ord) {
                case ASC: {
                    compar_ent_memb = compar_title_asc;
                    break;
                }
                case DESC: {
                    compar_ent_memb = compar_title_desc;
                    break;
                }
                case NO_SORT_ORD: {
                    return -1;
                }
            }
            break;
        }
        case STATUS: {
            switch (sort_ord) {
                case ASC: {
                    compar_ent_memb = compar_status_asc;
                    break;
                }
                case DESC: {
                    compar_ent_memb = compar_status_desc;
                    break;
                }
                case NO_SORT_ORD: {
                    return -1;
                }
            }
            break;
        }
        case SCORE: {
            switch (sort_ord) {
                case ASC: {
                    compar_ent_memb = compar_score_asc;
                    break;
                }
                case DESC: {
                    compar_ent_memb = compar_score_desc;
                    break;
                }
                case NO_SORT_ORD: {
                    return -1;
                }
            }
            break;
        }
        case PROG: {
            switch (sort_ord) {
                case ASC: {
                    compar_ent_memb = compar_prog_asc;
                    break;
                }
                case DESC: {
                    compar_ent_memb = compar_prog_desc;
                    break;
                }
                case NO_SORT_ORD: {
                    return -1;
                }
            }
            break;
        }
        case NO_ENT_MEMB: {
            return -1;
        }
    }
    qsort(ents, ent_num, sizeof(ent *), compar_ent_memb);
    return 0;
}

void free_ents(ent **ents, int ent_num) {
    for (int i = 0; i < ent_num; ++i) {
        free_ent(ents[i]);
    }
    free(ents);
}

ent **alloc_ents(int ent_num) {
    ent **ents = calloc(ent_num, sizeof(ent));
    for (int i = 0; i < ent_num; ++i) {
        ents[i] = alloc_ent();
        if (ents[i] == NULL) {
            free_ents(ents, i - 1);
            return NULL;
        }
    }
    return ents;
}