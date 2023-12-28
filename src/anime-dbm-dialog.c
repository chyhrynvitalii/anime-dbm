#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anime-dbm-db.h"
#include "anime-dbm-dialog.h"
#include "anime-dbm-rec.h"
#include "get.h"

// DESCRIPTION
//      number of database commands
#define db_cmnd_num     6

// DESCRIPTION
//      number of record commands
#define rec_cmnd_num    6

// DESCRIPTION
//      max length of a command
const size_t cmnd_len = 6;

// initializing close flags

int close_db_flag = 0;

int close_dbm_flag = 0;

// DESCRIPTION
//      gets db_cmnd
// RETURN VALUES
//      returns NO_DB_CMND on error, an actual db_cmnd on success
// ERRORS
//      sets errno to EINVAL if input is not a db_cmnd
enum db_cmnd get_db_cmnd() {
    char *cmnd = calloc(cmnd_len, sizeof(char));
    get_str(cmnd_len, "\nenter command (help to list commands): ", cmnd);

    if (strcasecmp(cmnd, "help") == 0) {
        free(cmnd);
        return DB_HELP;
    } else if (strcasecmp(cmnd, "new") == 0) {
        free(cmnd);
        return NEW_DB;
    } else if (strcasecmp(cmnd, "open") == 0) {
        free(cmnd);
        return OPEN_DB;
    } else if (strcasecmp(cmnd, "sort") == 0) {
        free(cmnd);
        return SORT_DB;
    } else if (strcasecmp(cmnd, "delete") == 0) {
        free(cmnd);
        return DEL_DB;
    }  else if (strcasecmp(cmnd, "close") == 0) {
        free(cmnd);
        return CLOSE_DBM;
    } else {
        free(cmnd);
        errno = EINVAL;
        return NO_DB_CMND;
    }
}

// DESCRIPTION
//      gets rec_cmnd
// RETURN VALUES
//      returns NO_REC_CMND on error, an actual rec_cmnd on success
// ERRORS
//      sets errno to EINVAL if input is not a rec_cmnd
enum rec_cmnd get_rec_cmnd() {
    char *cmnd = calloc(cmnd_len, sizeof(char));
    get_str(cmnd_len, "\nenter command (help to list commands): ", cmnd);

    if (strcasecmp(cmnd, "help") == 0) {
        free(cmnd);
        return REC_HELP;
    } else if (strcasecmp(cmnd, "new") == 0) {
        free(cmnd);
        return NEW_REC;
    } else if (strcasecmp(cmnd, "read") == 0) {
        free(cmnd);
        return READ_REC;
    } else if (strcasecmp(cmnd, "edit") == 0) {
        free(cmnd);
        return EDIT_REC;
    } else if (strcasecmp(cmnd, "delete") == 0) {
        free(cmnd);
        return DEL_REC;
    } else if (strcasecmp(cmnd, "close") == 0) {
        free(cmnd);
        return CLOSE_DB;
    } else {
        free(cmnd);
        errno = EINVAL;
        return NO_REC_CMND;
    }
}

// DESCRIPTION
//      lists database commands
// RETURN VALUES
//      returns 0 on success
int ls_db_cmnds() {
    puts("commands:\n"
         "help\tlist commands\n"
         "new\tnew database\n"
         "open\topen database\n"
         "sort\tsort database\n"
         "delete\tdelete database\n"
         "close\tclose application");
    return 0;
}

// DESCRIPTION
//      list record commands
// RETURN VALUES
//      returns 0 on success
int ls_rec_cmnds(char *) {
    puts("commands:\n"
         "help\tlist commands\n"
         "new\tnew record\n"
         "read\tread record\n"
         "edit\tedit record\n"
         "delete\tdelete record\n"
         "close\tclose database");
    return 0;
}

// DESCRIPTION
//      sets close_db_flag to 1
int close_db(char *) {
    close_db_flag = 1;
    return 0;
}

// DESCRIPTION
//      a lookup table for record commands
//      index of a command corresponds to rec_cmnd enumeration
int (*rec_cmnds[rec_cmnd_num])(char *db_name) = {ls_rec_cmnds,
                                                 new_rec,
                                                 read_rec,
                                                 edit_rec,
                                                 del_rec,
                                                 close_db};

int db_rec_dialog(char *db_name) {
    enum rec_cmnd cmnd = get_rec_cmnd();
    if (cmnd == NO_REC_CMND) {
        return -1;
    } else {
        return rec_cmnds[cmnd](db_name);
    }
}

// DESCRIPTION
//      sets close_dbm_flag to 1
int close_dbm() {
    close_dbm_flag = 1;
    return 0;
}

// DESCRIPTION
//      a lookup table for database commands
//      index of a command corresponds to db_cmnd enumeration
int (*db_cmnds[db_cmnd_num])() = {ls_db_cmnds,
                                  new_db,
                                  open_db,
                                  sort_db,
                                  del_db,
                                  close_dbm};

int db_dialog() {
    enum db_cmnd cmnd = get_db_cmnd();
    if (cmnd == NO_DB_CMND) {
        return -1;
    } else {
        return db_cmnds[cmnd]();
    }
}