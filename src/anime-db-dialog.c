#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "anime-db-dialog.h"
#include "anime-db-engine.h"
#include "get.h"
#include "file.h"

const size_t cmnd_len = 6;

// get commands

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
        return DB_NO_CMND;
    }
}

enum ent_cmnd get_ent_cmnd() {
    char *cmnd = calloc(cmnd_len, sizeof(char));
    get_str(cmnd_len, "\nenter command (help to list commands): ", cmnd);

    if (strcasecmp(cmnd, "help") == 0) {
        free(cmnd);
        return ENT_HELP;
    } else if (strcasecmp(cmnd, "new") == 0) {
        free(cmnd);
        return NEW_ENT;
    } else if (strcasecmp(cmnd, "read") == 0) {
        free(cmnd);
        return READ_ENT;
    } else if (strcasecmp(cmnd, "edit") == 0) {
        free(cmnd);
        return EDIT_ENT;
    } else if (strcasecmp(cmnd, "delete") == 0) {
        free(cmnd);
        return DEL_ENT;
    } else if (strcasecmp(cmnd, "close") == 0) {
        free(cmnd);
        return CLOSE_DB;
    } else {
        free(cmnd);
        errno = EINVAL;
        return ENT_NO_CMND;
    }
}

// list database commands
// returns -1 on error, 0 on success
int ls_db_cmnds() {
    if (puts("commands:\n"
             "help\tlist commands\n"
             "new\tnew database\n"
             "open\topen database\n"
             "sort\tsort database\n"
             "delete\tdelete database\n"
             "close\tclose application") == EOF) {
        return -1;
    } else {
        return 0;
    }
}

// list entry commands
// returns -1 on error, 0 on success
int ls_ent_cmnds() {
    if (puts("commands:\n"
             "help\tlist commands\n"
             "new\tnew entry\n"
             "read\tread entry\n"
             "edit\tedit entry\n"
             "delete\tdelete entry\n"
             "close\tclose database") == EOF) {
        return -1;
    } else {
        return 0;
    }
}

// entry dialog
// returns -1 on error, 0 on success, sets g_close_db to 1 on close
int ent_dialog(char *db_name) {
    // get entry command
    enum ent_cmnd cmnd = get_ent_cmnd();
    if (cmnd == ENT_NO_CMND) {
        return -1;
    }

    // TODO implement feedback for entry commands

    switch (cmnd) {
        case DB_HELP: {
            // list entry commands
            if (ls_ent_cmnds() == -1) {
                return -1;
            }

            return 0;
        }
        case NEW_ENT: {
            // TODO implement creating a new entry

            return 0;
        }
        case READ_ENT: {
            // TODO implement reading an entry

            return 0;
        }
        case EDIT_ENT: {
            // TODO implement editing an entry

            return 0;
        }
        case DEL_ENT: {
            // TODO implement deleting an entry

            return 0;
        }
        case CLOSE_DB: {
            close_db_flag = 1;

            return 0;
        }
    }
}

// database dialogue
// returns -1 on error, 0 on success, sets close_dbm_flag to 1 on close
int db_dialog() {
    // get db command
    enum db_cmnd cmnd = get_db_cmnd();
    if (cmnd == DB_NO_CMND) {
        return -1;
    }

    switch (cmnd) {
        case DB_HELP: {
            // list database commands
            if (ls_db_cmnds() == -1) {
                return -1;
            }

            return 0;
        }
        case NEW_DB: {
            // get database name
            char *db_name = calloc(FILENAME_MAX, sizeof(char));
            if (get_dbname(db_name) == -1) {
                return -1;
            }

            // create database
            FILE *db = fopen(db_name, "wx");
            if (db == NULL) {
                return -1;
            } else {
                printf("database %s has been created\n", db_name);
            }

            fclose(db);
            free(db_name);

            return 0;
        }
        case OPEN_DB: {
            // list databases in current directory
            int csv_ent_num = ls_select_dirent(".", select_csv);
            if (csv_ent_num == -1) {
                return -1;
            } else if (csv_ent_num == 0) {
                puts("there are no databases in the current directory");
                return 0;
            }

            // get database name
            char *db_name = calloc(FILENAME_MAX, sizeof(char));
            if (get_dbname(db_name) == -1) {
                return -1;
            }

            // check if database with such name exists
            if (access(db_name, F_OK) == -1) {
                return -1;
            }

            // open database
            close_db_flag = 0;
            printf("database %s has been opened\n", db_name);
            do {
                if (ent_dialog(db_name) == -1) {
                    perror("error");
                }
            } while (close_db_flag != 1);
            printf("database %s has been closed\n", db_name);

            free(db_name);

            return 0;
        }
        case SORT_DB: {
            // list databases in current directory
            int csv_ent_num = ls_select_dirent(".", select_csv);
            if (csv_ent_num == -1) {
                return -1;
            } else if (csv_ent_num == 0) {
                puts("there are no databases in the current directory");
                return 0;
            }

            // get database name
            char *db_name = calloc(FILENAME_MAX, sizeof(char));
            if (get_dbname(db_name) == -1) {
                return -1;
            }

            // check if database with such name exists
            if (access(db_name, F_OK) == -1) {
                return -1;
            }

            // TODO implement sorting databases
            /*if (sort(db_name) == -1) {
                return -1;
            } else {
                printf("database %s has been sorted", db_name);
            }*/

            free(db_name);

            return 0;
        }
        case DEL_DB: {
            // list databases in current directory
            int csv_ent_num = ls_select_dirent(".", select_csv);
            if (csv_ent_num == -1) {
                return -1;
            } else if (csv_ent_num == 0) {
                puts("there are no databases in the current directory");
                return 0;
            }

            // get database name
            char *db_name = calloc(FILENAME_MAX, sizeof(char));
            if (get_dbname(db_name) == -1) {
                return -1;
            }

            // check if database with such name exists
            if (access(db_name, F_OK) == -1) {
                return -1;
            }

            // delete database
            if (remove(db_name) == -1) {
                return -1;
            } else {
                printf("database %s has been deleted", db_name);
            }

            free(db_name);

            return 0;
        }
        case CLOSE_DBM: {
            close_dbm_flag = 1;

            return 0;
        }
    }
}