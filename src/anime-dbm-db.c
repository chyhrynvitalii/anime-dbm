#include <stdio.h>
#include <errno.h>

#include "anime-dbm-db.h"
#include "file.h"
#include "get.h"
#include "str.h"

// DESCRIPTION
//      current directory relative path
const char *cur_dir = ".";

// DESCRIPTION
//      csv file extension
const char *csv_file_ext = ".csv";

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