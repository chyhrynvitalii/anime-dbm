#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "anime-db-dialog.h"
#include "anime-db-engine.h"
#include "get.h"

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
    }  else if (strcasecmp(cmnd, "quit") == 0) {
        free(cmnd);
        return QUIT;
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

// list commands

void ls_db_cmnds() {
    puts("commands:\n"
         "help\tlist commands\n"
         "new\tnew database\n"
         "open\topen database\n"
         "sort\tsort database\n"
         "delete\tdelete database\n"
         "quit\tquit application");
}

void ls_ent_cmnds() {
    puts("commands:\n"
         "help\tlist commands\n"
         "new\tnew entry\n"
         "read\tread entry\n"
         "edit\tedit entry\n"
         "delete\tdelete entry\n"
         "close\tclose database");
}

// dialogs

void ent_dialog(char *db_name) {
    enum ent_cmnd cmnd;

    printf("database %s has been opened\n", db_name);

    do {
        // get entry command
        do {
            errno = 0;
            cmnd = get_ent_cmnd();
            if (cmnd == ENT_NO_CMND) {
                perror("error");
            }
        } while (errno != 0);

        // TODO implement feedback for entry commands

        switch (cmnd) {
            case DB_HELP: {
                ls_ent_cmnds();
                break;
            }
            case NEW_ENT: {
                // TODO implement creating a new entry

                break;
            }
            case READ_ENT: {
                // TODO implement reading an entry

                break;
            }
            case EDIT_ENT: {
                // TODO implement editing an entry

                break;
            }
            case DEL_ENT: {
                // TODO implement deleting an entry

                break;
            }
            default:
                break;
        }
    } while(cmnd != CLOSE_DB);

    printf("database %s has been closed\n", db_name);
}

void db_dialog() {
    enum db_cmnd cmnd;

    do {
        // get db command
        do {
            errno = 0;
            cmnd = get_db_cmnd();
            if (cmnd == DB_NO_CMND) {
                perror("error");
            }
        } while (errno != 0);

        // TODO implement feedback for database commands

        switch (cmnd) {
            case DB_HELP: {
                ls_db_cmnds();
                break;
            }
            case NEW_DB: {
                char *db_name = calloc(FILENAME_MAX, sizeof(char));
                if (get_dbname(db_name) == -1) {
                    perror("error");
                    break;
                }

                FILE *db = fopen(db_name, "wx");
                if (db == NULL) {
                    perror("error");
                    break;
                }

                fclose(db);
                free(db_name);
                break;
            }
            case OPEN_DB: {
                char *db_name = calloc(FILENAME_MAX, sizeof(char));
                if (get_dbname(db_name) == -1) {
                    perror("error");
                    break;
                }

                if (access(db_name, F_OK) == -1) {
                    perror("error");
                    break;
                }

                ent_dialog(db_name);

                free(db_name);
                break;
            }
            case SORT_DB: {
                char *db_name = calloc(FILENAME_MAX, sizeof(char));
                if (get_dbname(db_name) == -1) {
                    perror("error");
                    break;
                }

                if (access(db_name, F_OK) == -1) {
                    perror("error");
                    break;
                }

                // TODO implement sorting databases

                free(db_name);
                break;
            }
            case DEL_DB: {
                char *db_name = calloc(FILENAME_MAX, sizeof(char));
                if (get_dbname(db_name) == -1) {
                    perror("error");
                    break;
                }

                if (access(db_name, F_OK) == -1) {
                    perror("error");
                    break;
                }

                if (remove(db_name) == -1) {
                    perror("error");
                }

                free(db_name);
                break;
            }
            default:
                break;
        }
    } while (cmnd != QUIT);
}