#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "anime-dbm-db.h"
#include "anime-dbm-dialog.h"
#include "anime-dbm-rec.h"
#include "anime-dbm-sort.h"
#include "file.h"
#include "get.h"
#include "str.h"

// DESCRIPTION
//      current directory relative path
const char *cur_dir = ".";

// DESCRIPTION
//      csv file extension
const char *csv_file_ext = ".csv";

// DESCRIPTION
//      lists databases in the current directory or tells that there are no databases
// RETURN VALUES
//      returns number of databases listed
int ls_dbs() {
    int db_num = ls_select_dir_ent(cur_dir, select_csv);
    if (db_num == 0) {
        puts("there are no databases in the current directory");
    }
    return db_num;
}

// DESCRIPTION
//      gets database name, stores it in db_name_buf
// RETURN VALUES
//      returns -1 on error, 0 on success
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

int new_db() {
    char *db_name = calloc(FILENAME_MAX, sizeof(char));
    if (get_db_name(db_name) == -1) {
        return -1;
    }

    FILE *db = fopen(db_name, "wx");
    if (db == NULL) {
        free(db_name);
        return -1;
    } else {
        printf("database %s has been created\n", db_name);
        free(db_name);
        fclose(db);
        return 0;
    }
}

int open_db() {
    if (ls_dbs() == 0) {
        return 0;
    }

    char *db_name = calloc(FILENAME_MAX, sizeof(char));
    if (get_db_name(db_name) == -1) {
        return -1;
    }

    if (access(db_name, F_OK) == -1) {
        free(db_name);
        return -1;
    } else {
        close_db_flag = 0;
        printf("database %s has been opened\n", db_name);
        do {
            if (db_rec_dialog(db_name) == -1) {
                perror("error");
            }
        } while (close_db_flag != 1);
        printf("database %s has been closed\n", db_name);
        free(db_name);
        return 0;
    }
}

int sort_db() {
    if (ls_dbs() == 0) {
        return 0;
    }

    char *db_name = calloc(FILENAME_MAX, sizeof(char));
    if (get_db_name(db_name) == -1) {
        return -1;
    }

    int rec_num = get_rec_num_csv(db_name);
    if (rec_num == -1) {
        free(db_name);
        return -1;
    } else if (rec_num == 0) {
        puts("there are no records in the database");
        free(db_name);
        return 0;
    }

    rec **recs = alloc_recs(rec_num);
    if (scan_recs(db_name, recs, rec_num) == -1) {
        free(db_name);
        free_recs(recs, rec_num);
        return -1;
    }

    enum rec_key rec_key = get_rec_key();
    if (rec_key == NO_REC_KEY) {
        free(db_name);
        free_recs(recs, rec_num);
        return -1;
    }

    enum sort_ord sort_ord = get_sort_ord();
    if (sort_ord == NO_SORT_ORD) {
        free(db_name);
        free_recs(recs, rec_num);
        return -1;
    }

    qsort(recs, rec_num, sizeof(rec *), rec_key_compars[rec_key][sort_ord]);

    if (write_recs(db_name, recs, rec_num) == -1) {
        free(db_name);
        free_recs(recs, rec_num);
        return -1;
    } else {
        printf("database %s has been sorted\n", db_name);
        free(db_name);
        free_recs(recs, rec_num);
        return 0;
    }
}

int del_db() {
    if (ls_dbs() == 0) {
        return 0;
    }

    char *db_name = calloc(FILENAME_MAX, sizeof(char));
    if (get_db_name(db_name) == -1) {
        return -1;
    }

    if (remove(db_name) == -1) {
        free(db_name);
        return -1;
    } else {
        printf("database %s has been deleted\n", db_name);
        free(db_name);
        return 0;
    }
}