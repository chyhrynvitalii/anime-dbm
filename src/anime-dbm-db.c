#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "anime-dbm-db.h"
#include "file.h"
#include "get.h"
#include "str.h"
#include "anime-dbm-dialog.h"

// DESCRIPTION
//      current directory relative path
const char *cur_dir = ".";

// DESCRIPTION
//      csv file extension
const char *csv_file_ext = ".csv";

int ls_dbs() {
    int db_num = ls_select_dir_ent(cur_dir, select_csv);
    if (db_num == 0) {
        puts("there are no databases in the current directory");
    }
    return db_num;
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

int delete_db() {
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