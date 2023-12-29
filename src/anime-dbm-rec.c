#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anime-dbm-conf.h"
#include "anime-dbm-rec.h"
#include "file.h"
#include "get.h"
#include "maths.h"
#include "anime-dbm-sort.h"

// DESCRIPTION
//      max length of a record key as a string
const size_t rec_key_len = 8;

// DESCRIPTION
//      max length of a record value as a string
const size_t title_len = 128, status_len = 16, score_len = 4, prog_len = 4;

// DESCRIPTION
//      domain of score
const float score_low = 0, score_up = 10;

// DESCRIPTION
//      record csv print format
const char *rec_printf_csv = "\"%s\",\"%s\",%.2f,%u\n";

// DESCRIPTION
//      record csv scan format
const char *rec_scanf_csv = "\"%[^\"]\",\"%[^\"]\",%f,%u%*c";

// DESCRIPTION
//     record toml print format
const char *rec_printf_toml = "[%s]\n"
                              "status = %s\n"
                              "score = %.2f\n"
                              "progress = %u\n";

// DESCRIPTION
//      returns a pointer to memory allocated for a record
rec *alloc_rec() {
    rec *rec_ptr = calloc(1, sizeof(rec));
    rec_ptr->title = calloc(title_len, sizeof(char));
    rec_ptr->status = calloc(status_len, sizeof(char));
    return rec_ptr;
}

// DESCRIPTION
//      deallocates memory allocated for a record pointed to by rec
void free_rec(rec *rec) {
    free(rec->title);
    free(rec->status);
    free(rec);
}

// DESCRIPTION
//      returns a pointer to memory allocated for rec_num record pointers
rec **alloc_recs(int rec_num) {
    rec **recs = calloc(rec_num, sizeof(rec *));
    for (int i = 0; i < rec_num; ++i) {
        recs[i] = alloc_rec();
    }
    return recs;
}

// DESCRIPTION
//      deallocates memory allocated for rec_num record pointers pointed to by recs
void free_recs(rec **recs, int rec_num) {
    for (int i = 0; i < rec_num; ++i) {
        free_rec(recs[i]);
    }
    free(recs);
}

// DESCRIPTION
//      gets a title, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec_title(rec *rec) {
    if (get_str(title_len, "title: ", rec->title) == -1) {
        return -1;
    } else {
        return 0;
    }
}

// DESCRIPTION
//      gets status, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec_status(rec *rec) {
    if (get_str(status_len, "status: ", rec->status) == -1) {
        return -1;
    } else {
        return 0;
    }
}

// DESCRIPTION
//      gets score, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
// ERRORS
//      sets errno to EDOM if input does not belong to the domain of score
int get_rec_score(rec *rec) {
    if (get_float(score_len, "score: ", &rec->score) == -1) {
        return -1;
    } else if (!in_closed_inter(rec->score, score_low, score_up)) {
        errno = EDOM;
        return -1;
    } else {
        return 0;
    }
}

// DESCRIPTION
//      gets progress, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec_prog(rec *rec) {
    if (get_uint(prog_len, "progress: ", &rec->prog) == -1) {
        return -1;
    } else {
        return 0;
    }
}

// DESCRIPTION
//      gets a record, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec(rec *rec) {
    if (get_rec_title(rec) == -1) {
        return -1;
    } else if (get_rec_status(rec) == -1) {
        return -1;
    } else if (get_rec_score(rec) == -1) {
        return -1;
    } else if (get_rec_prog(rec) == -1) {
        return -1;
    } else {
        return 0;
    }
}

// DESCRIPTION
//      appends a record pointer to by rec to a database db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
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

// DESCRIPTION
//      writes rec_num records pointed to by recs to database db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
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

// DESCRIPTION
//      scans database db_name for rec_num records, stores scanned records in recs
// RETURN VALUES
//      returns -1 on error, 0 on success
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

// DESCRIPTION
//      lists titles of rec_num records pointed to by recs
void ls_rec_titles(rec **recs, int rec_num) {
    for (int i = 0; i < rec_num; ++i) {
        puts(recs[i]->title);
    }
}

// DESCRIPTION
//      gets title of a record as a string, stores it in rec_title_buf
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec_title_str(char *rec_title_buf) {
    if (get_str(title_len, "title: ", rec_title_buf) == -1) {
        return -1;
    } else {
        return 0;
    }
}

// DESCRIPTION
//      gets a record from rec_num records pointed to by recs with the same title as rec_title
// RETURN VALUES
//      returns the matching record or NULL if a matching record has not been found
rec *get_match_title_rec(rec **recs, const char *rec_title, int rec_num) {
    for (int i = 0; i < rec_num; ++i) {
        if (strcmp(recs[i]->title, rec_title) == 0) {
            return recs[i];
        }
    }
    return NULL;
}

// DESCRIPTION
//      gets the record title as a string and finds the record with the same name from rec_num records pointed to by
//      recs
// RETURN VALUES
//      returns the record with the same title as the entered title or NULL if couldn't get the record title or if the
//      record with the same title does not exist
// ERRORS
//      sets errno to EINVAL if the record with the same title does not exist
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

// DESCRIPTION
//      prints out the record to stdout as toml
void put_rec(rec *rec) {
    printf(rec_printf_toml, rec->title, rec->status, rec->score, rec->prog);
}

// DESCRIPTION
//      gets a rec_key
// RETURN VALUES
//      returns NO_REC_KEY on error, an actual rec_key on success
// ERRORS
//      sets errno to EINVAL if input is not a rec_key
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

// DESCRIPTION
//      a lookup table for record value getters
int (*rec_key_gets[rec_key_num])(rec *rec) = {get_rec_title,
                                              get_rec_status,
                                              get_rec_score,
                                              get_rec_prog};

// DESCRIPTION
//      prompts the user to edit the value of rec_key of record pointed to by rec
int edit_rec_key(rec *rec, enum rec_key rec_key) {
    return rec_key_gets[rec_key](rec);
}

// DESCRIPTION
//      keeps sorting in the database
// RETURN VALUES
//      returns -1 on error, 0 on success
int keep_sort(char *db_name) {
    int db_num = get_db_num();
    conf_rec **conf_recs = calloc_conf_recs(db_num);
    if (scan_conf_recs(conf_recs, db_num) == -1) {
        return -1;
    }

    conf_rec *target_conf_rec = get_target_conf_rec(conf_recs, db_num, db_name);
    if (target_conf_rec == NULL) {
        return -1;
    }

    int rec_num = get_rec_num_csv(db_name);
    rec **recs = alloc_recs(rec_num);
    if (scan_recs(db_name, recs, rec_num) == -1) {
        free_recs(recs, rec_num);
        return -1;
    }

    if (target_conf_rec->rec_key != NO_REC_KEY && target_conf_rec->sort_ord != NO_SORT_ORD) {
        qsort(recs, rec_num, sizeof(rec *),
              rec_key_compars[target_conf_rec->rec_key][target_conf_rec->sort_ord]);
    }

    if (write_recs(db_name, recs, rec_num) == -1) {
        return -1;
    } else {
        free_recs(recs, rec_num);
        return 0;
    }
}

int new_rec(char *db_name) {
    rec *new_rec = alloc_rec();

    if (get_rec(new_rec) == -1) {
        free_rec(new_rec);
        return -1;
    }

    if (append_rec(db_name, new_rec) == -1) {
        free_rec(new_rec);
        return -1;
    } else {
        printf("record %s has been created\n", new_rec->title);
        free_rec(new_rec);
    }

    if (keep_sort(db_name) == -1) {
        return -1;
    } else {
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
    }

    if (keep_sort(db_name) == -1) {
        free_recs(recs, rec_num);
        return -1;
    } else {
        printf("the record %s has been edited\n", target_rec->title);
        free_recs(recs, rec_num);
        return 0;
    }
}

int sort_recs(char *db_name) {
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

    enum rec_key rec_key = get_rec_key();
    if (rec_key == NO_REC_KEY) {
        free_recs(recs, rec_num);
        return -1;
    }

    enum sort_ord sort_ord = get_sort_ord();
    if (sort_ord == NO_SORT_ORD) {
        free_recs(recs, rec_num);
        return -1;
    }

    qsort(recs, rec_num, sizeof(rec *), rec_key_compars[rec_key][sort_ord]);

    if (write_recs(db_name, recs, rec_num) == -1) {
        free_recs(recs, rec_num);
        return -1;
    } else {
        printf("database %s has been sorted\n", db_name);
        free_recs(recs, rec_num);
    }

    if (update_conf(db_name, rec_key, sort_ord) == -1) {
        return -1;
    } else {
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
    }

    if (keep_sort(db_name) == -1) {
        return -1;
    } else {
        return 0;
    }
}