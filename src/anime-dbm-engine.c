#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "anime-dbm-engine.h"
#include "get.h"
#include "maths.h"
#include "str.h"
#include "file.h"
#include "anime-dbm-dialog.h"

// constant file names
const char *cur_dir = ".";
const char *log_name = "anime-dbm.log";

// file extensions
const char *csv_file_ext = ".csv";
const char *log_file_ext = ".log";

// length of database entry keys
const size_t title_len = 128, status_len = 16, score_len = 4, prog_len = 4, ent_key_len = 8, sort_ord_len = 16;

// domain of score
const float score_low = 0, score_up = 10;

// formats
const char *db_ent_printf_csv = "\"%s\",\"%s\",%.2f,%u\n";

const char *db_ent_scanf_csv = "\"%[^\"]\",\"%[^\"]\",%f,%u%*c";

const char *db_ent_printf_toml = "[%s]\n"
                                 "status = \"%s\"\n"
                                 "score = %.2f\n"
                                 "progress = %u\n";

const char *db_ent_title_printf_toml = "[%s]\n";

const char *db_ent_key_ls_toml = "[title]\n"
                                 "status\n"
                                 "score\n"
                                 "progress\n";

const char *log_ent_printf_csv = "\"%s\",%i,%i\n";

const char *log_ent_scanf_csv = "\"%[^\"]\",%i,%i%*c";

db_ent *alloc_db_ent() {
    db_ent *db_ent = calloc(1, sizeof(*db_ent));
    if ((db_ent->title = calloc(title_len, sizeof(char))) == NULL) {
        return NULL;
    }
    if ((db_ent->status = calloc(status_len, sizeof(char))) == NULL) {
        return NULL;
    }
    return db_ent;
}

void free_db_ent(db_ent *db_ent) {
    free(db_ent->title);
    free(db_ent->status);
}

int get_db_ent_title(db_ent *db_ent) {
    if (get_str(title_len, "title: ", db_ent->title) == -1) {
        return -1;
    }
    return 0;
}

int get_db_ent_status(db_ent *db_ent) {
    if (get_str(status_len, "status: ", db_ent->status) == -1) {
        return -1;
    }
    return 0;
}

int get_db_ent_score(db_ent *db_ent) {
    if (get_float(score_len, "score: ", &db_ent->score) == -1) {
        return -1;
    } else if (!in_closed_range(db_ent->score, score_low, score_up)) {
        errno = EDOM;
        return -1;
    }
    return 0;
}

int get_db_ent_prog(db_ent *db_ent) {
    if (get_uint(prog_len, "progress: ", &db_ent->prog) == -1) {
        return -1;
    }
    return 0;
}

int get_db_ent(db_ent *db_ent) {
    if (get_db_ent_title(db_ent) == -1) {
        return -1;
    }
    if (get_db_ent_status(db_ent) == -1) {
        return -1;
    }
    if (get_db_ent_score(db_ent) == -1) {
        return -1;
    }
    if (get_db_ent_prog(db_ent) == -1) {
        return -1;
    }
    return 0;
}

int get_db_name(char *db_name_buf) {
    printf("enter database name (must be a %s file): ", csv_file_ext);
    if (get_str(FILENAME_MAX, NULL, db_name_buf) == -1) {
        return -1;
    } else if (!ends_substr(db_name_buf, csv_file_ext)) {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

int append_db_ent(char *db_name, db_ent *db_ent) {
    FILE *db = fopen(db_name, "a");
    if (db == NULL) {
        return -1;
    }
    if (fprintf(db, db_ent_printf_csv, db_ent->title, db_ent->status, db_ent->score, db_ent->prog) < 0) {
        return -1;
    }
    if (fclose(db) == EOF) {
        return -1;
    }
    return 0;
}

int get_db_ent_num(char *db_name) {
    int ent_num = 0;
    FILE *db = fopen(db_name, "r");
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

int scan_db(char *db_name, const char *scan_format, db_ent **db_ent, int ent_num) {
    FILE *db = fopen(db_name, "r");
    if (db == NULL) {
        return -1;
    }
    for (int i = 0; i < ent_num; ++i) {
        fscanf(db, scan_format, db_ent[i]->title, db_ent[i]->status, &db_ent[i]->score, &db_ent[i]->prog);
    }
    fclose(db);
    return 0;
}

void printf_db_ent(db_ent *db_ent, const char *print_format) {
    printf(print_format, db_ent->title, db_ent->status, db_ent->score, db_ent->prog);
}

void ls_titles(db_ent **ents, const char *print_format, int ent_num) {
    for (int i = 0; i < ent_num; ++i) {
        printf(print_format, ents[i]->title);
    }
}

db_ent *get_match_title_db_ent(db_ent **db_ents, const char *db_ent_title, int db_ent_num) {
    for (int i = 0; i < db_ent_num; ++i) {
        if (strcmp(db_ents[i]->title, db_ent_title) == 0) {
            return db_ents[i];
        }
    }
    return NULL;
}

int erase_db(char *db_name) {
    FILE *db = fopen(db_name, "w");
    if (db == NULL) {
        return -1;
    }
    return 0;
}

enum db_sort_ord get_sort_ord() {
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

enum db_ent_key get_db_ent_key() {
    char *ent_key = calloc(title_len, sizeof(char));
    if (get_str(ent_key_len, "entry key: ", ent_key) == -1) {
        return -1;
    }
    if (strcasecmp(ent_key, "title") == 0) {
        free(ent_key);
        return TITLE;
    } else if (strcasecmp(ent_key, "status") == 0) {
        free(ent_key);
        return STATUS;
    } else if (strcasecmp(ent_key, "score") == 0) {
        free(ent_key);
        return SCORE;
    } else if (strcasecmp(ent_key, "progress") == 0) {
        free(ent_key);
        return PROG;
    } else {
        free(ent_key);
        errno = EINVAL;
        return NO_ENT_KEY;
    }
}

int compar_title_asc(const void *ent1, const void *ent2) {
    db_ent *ent1_cast = *(db_ent **)ent1;
    db_ent *ent2_cast = *(db_ent **)ent2;
    return strcmp(ent1_cast->title, ent2_cast->title);
}

int compar_status_asc(const void *ent1, const void *ent2) {
    db_ent *ent1_cast = *(db_ent **)ent1;
    db_ent *ent2_cast = *(db_ent **)ent2;
    return strcmp(ent1_cast->status, ent2_cast->status);
}

int compar_score_asc(const void *ent1, const void *ent2) {
    db_ent *ent1_cast = *(db_ent **)ent1;
    db_ent *ent2_cast = *(db_ent **)ent2;
    if (ent1_cast->score > ent2_cast->score) {
        return 1;
    } else if (ent1_cast->score < ent2_cast->score) {
        return -1;
    } else {
        return 0;
    }
}

int compar_prog_asc(const void *ent1, const void *ent2) {
    db_ent *ent1_cast = *(db_ent **)ent1;
    db_ent *ent2_cast = *(db_ent **)ent2;
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

int (*compar_ent_key_lut[ent_key_num][sort_ord_num])(const void *, const void *) =
        {{compar_title_asc, compar_title_desc},
         {compar_status_asc, compar_status_desc},
         {compar_score_asc, compar_score_desc},
         {compar_prog_asc, compar_prog_desc}};

void sort_db_ents(db_ent **db_ents, int db_ent_num, enum db_ent_key db_ent_key, enum db_sort_ord db_sort_ord) {
    qsort(db_ents, db_ent_num, sizeof(db_ent *), compar_ent_key_lut[db_ent_key][db_sort_ord]);
}

void free_db_ents(db_ent **db_ents, int db_ent_num) {
    for (int i = 0; i < db_ent_num; ++i) {
        free_db_ent(db_ents[i]);
    }
    free(db_ents);
}

db_ent **alloc_db_ents(int ent_num) {
    db_ent **ents = calloc(ent_num, sizeof(db_ent));
    for (int i = 0; i < ent_num; ++i) {
        ents[i] = alloc_db_ent();
        if (ents[i] == NULL) {
            free_db_ents(ents, i - 1);
            return NULL;
        }
    }
    return ents;
}

int new_log() {
    // create database
    FILE *db = fopen(log_name, "wx");
    if (db == NULL) {
        return -1;
    }
    return 0;
}

int log_sort_state();

int scan_log(char *logname, const char *scan_format, log_ent **log_ents, int log_ent_num) {
    FILE *log = fopen(logname, "r");
    if (log == NULL) {
        return -1;
    }
    for (int i = 0; i < log_ent_num; ++i) {
        fscanf(log, scan_format, log_ents[i]->db_name, &log_ents[i]->sort_ord, &log_ents[i]->ent_key);
    }
    fclose(log);
    return 0;
}

int new_db(const char *db_name) {
    FILE *db = fopen(db_name, "wx");
    if (db == NULL) {
        return -1;
    } else {
        fclose(db);
        return 0;
    }
}

int ls_dbs() {
    int db_num = ls_select_dir_ent(cur_dir, select_csv);
    if (db_num == -1) {
        return -1;
    } else if (db_num == 0) {
        puts("there are no databases in the current directory");
    }
    return 0;
}

int open_db(const char *db_name) {
    // check if database with the given name exists
    if (access(db_name, F_OK) == -1) {
        return -1;
    }

    // set state
    close_db_flag = 0;

    // call database entry dialog in a loop
    do {
        if (db_ent_dialog(db_name) == -1) {
            perror("error");
        }
    } while (close_db_flag != 1);

    return 0;
}