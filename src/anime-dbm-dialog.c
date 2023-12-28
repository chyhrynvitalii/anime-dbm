#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "anime-dbm-db.h"
#include "anime-dbm-dialog.h"
#include "anime-dbm-rec.h"
#include "file.h"
#include "get.h"

// initializing close flags
int close_db_flag = 0;
int close_dbm_flag = 0;

// DESCRIPTION
//      max length of a command
const size_t cmnd_len = 6;

// DESCRIPTION
//      gets db_cmnd
// RETURN VALUES
//      returns DB_NO_CMND on error, an actual db_cmnd on success
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
    } else if (strcasecmp(cmnd, "delete") == 0) {
        free(cmnd);
        return DEL_DB;
    }  else if (strcasecmp(cmnd, "close") == 0) {
        free(cmnd);
        return CLOSE_DBM;
    } else {
        free(cmnd);
        errno = EINVAL;
        return DB_NO_CMND;
    }
}

// DESCRIPTION
//      gets rec_cmnd
// RETURN VALUES
//      returns REC_NO_CMND on error, an actual rec_cmnd on success
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
    } else if (strcasecmp(cmnd, "sort") == 0) {
        free(cmnd);
        return SORT_DB;
    } else if (strcasecmp(cmnd, "close") == 0) {
        free(cmnd);
        return CLOSE_DB;
    } else {
        free(cmnd);
        errno = EINVAL;
        return REC_NO_CMND;
    }
}

// DESCRIPTION
//      lists database commands
// RETURN VALUES
//      returns -1 on error, 0 on success
void ls_db_cmnds() {
    puts("commands:\n"
         "help\tlist commands\n"
         "new\tnew database\n"
         "open\topen database\n"
         "delete\tdelete database\n"
         "close\tclose application");
}

// DESCRIPTION
//      list record commands
// RETURN VALUES
//      returns -1 on error, 0 on success
void ls_rec_cmnds() {
    puts("commands:\n"
         "help\tlist commands\n"
         "new\tnew record\n"
         "read\tread record\n"
         "edit\tedit record\n"
         "delete\tdelete record\n"
         "sort\tsort database\n"
         "close\tclose database");
}

int db_rec_dialog(char *db_name) {
    enum rec_cmnd cmnd = get_rec_cmnd();

    switch (cmnd) {
        // TODO implement keeping sorting
        case DB_HELP: {
            ls_rec_cmnds();
        }
        case NEW_REC: {
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
        case READ_REC: {
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
        case EDIT_REC: {
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
        case DEL_REC: {
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
        case SORT_DB: {
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

            qsort(recs, rec_num, sizeof(rec *), compar_rec_key_lut[rec_key][sort_ord]);

            if (write_recs(db_name, recs, rec_num) == -1) {
                free_recs(recs, rec_num);
                return -1;
            } else {
                printf("database %s has been sorted\n", db_name);
                free_recs(recs, rec_num);
                return 0;
            }
        }
        case CLOSE_DB: {
            close_db_flag = 1;
            return 0;
        }
        case REC_NO_CMND: {
            return -1;
        }
    }
}

int db_dialog() {
    enum db_cmnd cmnd = get_db_cmnd();

    switch (cmnd) {
        case DB_HELP: {
            ls_db_cmnds();
        }
        case NEW_DB: {
            char *db_name = calloc(FILENAME_MAX, sizeof(char));
            if (get_db_name(db_name) == -1) {
                return -1;
            }

            if (new_db(db_name) == -1) {
                free(db_name);
                return -1;
            } else {
                printf("database %s has been created\n", db_name);
                free(db_name);
                return 0;
            }
        }
        case OPEN_DB: {
            if (ls_dbs() == -1) {
                return -1;
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
                do {
                    if (db_rec_dialog(db_name) == -1) {
                        perror("error");
                    }
                } while (close_db_flag != 1);
                free(db_name);
                return 0;
            }
        }
        case DEL_DB: {
            if (ls_dbs() == -1) {
                return -1;
            }

            char *db_name = calloc(FILENAME_MAX, sizeof(char));
            if (get_db_name(db_name) == -1) {
                return -1;
            }

            if (remove(db_name) == -1) {
                free(db_name);
                return -1;
            } else {
                printf("database %s has been deleted", db_name);
                free(db_name);
                return 0;
            }
        }
        case CLOSE_DBM: {
            close_dbm_flag = 1;
            return 0;
        }
        case DB_NO_CMND: {
            return -1;
        }
    }
}