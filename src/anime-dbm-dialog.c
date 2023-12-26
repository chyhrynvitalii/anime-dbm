#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "anime-dbm-dialogs.h"
#include "anime-dbm-engine.h"
#include "get.h"

// DESCRIPTION
//      max length of a command
const size_t cmnd_len = 6;

// DESCRIPTION
//      gets db_cmnd
// RETURN VALUES
//      returns DB_NO_CMND on error, an actual db_cmnd on success
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

// DESCRIPTION
//      gets db_ent_cmnd
// RETURN VALUES
//      returns DB_ENT_NO_CMND on error, an actual db_ent_cmnd on success
enum db_ent_cmnd get_ent_cmnd() {
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
        return DB_ENT_NO_CMND;
    }
}

void ls_db_cmnds() {
    puts("commands:\n"
         "help\tlist commands\n"
         "new\tnew database\n"
         "open\topen database\n"
         "sort\tsort database\n"
         "delete\tdelete database\n"
         "close\tclose application");
}

void ls_db_ent_cmnds() {
    puts("commands:\n"
         "help\tlist commands\n"
         "new\tnew entry\n"
         "read\tread entry\n"
         "edit\tedit entry\n"
         "delete\tdelete entry\n"
         "close\tclose database");
}

int db_ent_dialog(char *db_name) {
    enum db_ent_cmnd cmnd = get_ent_cmnd();

    switch (cmnd) {
        // TODO implement keeping sorting
        // TODO refactor this mess as well
        case DB_HELP: {
            ls_db_ent_cmnds();
        }
        case NEW_ENT: {
            db_ent *ent = alloc_db_ent();
            if (ent == NULL) {
                return -1;
            }
            if (get_db_ent(ent) == -1) {
                free_db_ent(ent);
                return -1;
            }
            if (append_db_ent(db_name, ent) == -1) {
                free_db_ent(ent);
                return -1;
            }
            printf("entry %s has been created", ent->title);
            free_db_ent(ent);
            return 0;
        }
        case READ_ENT: {
            int ent_num = get_db_ent_num(db_name);
            if (ent_num == -1) {
                return -1;
            } else if (ent_num == 0) {
                puts("there are no entries in the database");
                return 0;
            }
            db_ent **ents = alloc_db_ents(ent_num);
            if (ents == NULL) {
                return -1;
            }
            if (scan_db(db_name, db_ent_scanf_csv, ents, ent_num) == -1) {
                free_db_ents(ents, ent_num);
                return -1;
            }
            ls_titles(ents, db_ent_title_printf_toml, ent_num);
            char *ent_title = calloc(title_len, sizeof(char));
            if (get_str(title_len, "title: ", ent_title) == -1) {
                free_db_ents(ents, ent_num);
                free(ent_title);
                return -1;
            }
            db_ent *chos_ent;
            if ((chos_ent = get_match_title_db_ent(ents, ent_title, ent_num)) == NULL) {
                free_db_ents(ents, ent_num);
                free(ent_title);
                errno = EINVAL;
                return -1;
            }
            printf_db_ent(chos_ent, db_ent_printf_toml);
            for (int i = 0; i < ent_num; ++i) {
                free_db_ent(ents[i]);
            }
            free(ents);
            free(ent_title);
            return 0;
        }
        case EDIT_ENT: {
            int ent_num = get_db_ent_num(db_name);
            if (ent_num == -1) {
                return -1;
            } else if (ent_num == 0) {
                puts("there are no entries in the database");
                return 0;
            }
            db_ent **ents = alloc_db_ents(ent_num);
            if (ents == NULL) {
                return -1;
            }
            if (scan_db(db_name, db_ent_scanf_csv, ents, ent_num) == -1) {
                return -1;
            }
            ls_titles(ents, db_ent_title_printf_toml, ent_num);
            char *chosen_ent_title = calloc(title_len, sizeof(char));
            if (get_str(title_len, "title: ", chosen_ent_title) == -1) {
                free_db_ents(ents, ent_num);
                free(chosen_ent_title);
                return -1;
            }
            db_ent *chos_ent;
            if ((chos_ent = get_match_title_db_ent(ents, chosen_ent_title, ent_num)) == NULL) {
                free_db_ents(ents, ent_num);
                free(chosen_ent_title);
                errno = EINVAL;
                return -1;
            }
            printf("%s", db_ent_key_ls_toml);
            enum db_ent_key chos_ent_key = get_db_ent_key();
            switch (chos_ent_key) {
                case TITLE: {
                    if (get_db_ent_title(chos_ent) == -1) {
                        free_db_ents(ents, ent_num);
                        free(chosen_ent_title);
                        return -1;
                    }
                    break;
                }
                case STATUS: {
                    if (get_db_ent_status(chos_ent) == -1) {
                        free_db_ents(ents, ent_num);
                        free(chosen_ent_title);
                        return -1;
                    }
                    break;
                }
                case SCORE: {
                    if (get_db_ent_score(chos_ent) == -1) {
                        free_db_ents(ents, ent_num);
                        free(chosen_ent_title);
                        return -1;
                    }
                    break;
                }
                case PROG: {
                    if (get_db_ent_prog(chos_ent) == -1) {
                        free_db_ents(ents, ent_num);
                        free(chosen_ent_title);
                        return -1;
                    }
                    break;
                }
                case NO_ENT_KEY: {
                    free_db_ents(ents, ent_num);
                    free(chosen_ent_title);
                    return -1;
                }
            }
            if (erase_db(db_name) == -1) {
                free_db_ents(ents, ent_num);
                free(chosen_ent_title);
                return -1;
            }
            for (int i = 0; i < ent_num; ++i) {
                if (append_db_ent(db_name, ents[i]) == -1) {
                    free_db_ents(ents, ent_num);
                    free(chosen_ent_title);
                    return -1;
                }
            }
            printf("entry %s has been edited", chosen_ent_title);
            for (int i = 0; i < ent_num; ++i) {
                free_db_ent(ents[i]);
            }
            free(ents);
            free(chosen_ent_title);
            return 0;
        }
        case DEL_ENT: {
            int ent_num = get_db_ent_num(db_name);
            if (ent_num == -1) {
                return -1;
            } else if (ent_num == 0) {
                puts("there are no entries in the database");
                return 0;
            }
            db_ent **ents = alloc_db_ents(ent_num);
            if (ents == NULL) {
                return -1;
            }
            if (scan_db(db_name, db_ent_scanf_csv, ents, ent_num) == -1) {
                free_db_ents(ents, ent_num);
                return -1;
            }
            ls_titles(ents, db_ent_title_printf_toml, ent_num);
            char *chos_ent_title = calloc(title_len, sizeof(char));
            if (get_str(title_len, "title: ", chos_ent_title) == -1) {
                free_db_ents(ents, ent_num);
                return -1;
            }
            db_ent *chos_ent;
            if ((chos_ent = get_match_title_db_ent(ents, chos_ent_title, ent_num)) == NULL) {
                free_db_ents(ents, ent_num);
                free(chos_ent_title);
                errno = EINVAL;
                return -1;
            }
            if (erase_db(db_name) == -1) {
                free_db_ents(ents, ent_num);
                free(chos_ent_title);
                return -1;
            }
            for (int i = 0; i < ent_num; ++i) {
                if (ents[i] != chos_ent) {
                    if (append_db_ent(db_name, ents[i]) == -1) {
                        free_db_ents(ents, ent_num);
                        free(chos_ent_title);
                        return -1;
                    }
                }
            }
            printf("entry %s has been deleted", chos_ent_title);
            for (int i = 0; i < ent_num; ++i) {
                free_db_ent(ents[i]);
            }
            free(ents);
            free(chos_ent_title);
            return 0;
        }
        case CLOSE_DB: {
            close_db_flag = 1;
            return 0;
        }
        case DB_ENT_NO_CMND: {
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
            }

            close_db_flag = 0;

            do {
                if (db_ent_dialog(db_name) == -1) {
                    perror("error");
                }
            } while (close_db_flag != 1);

            free(db_name);
            return 0;
        }
        case SORT_DB: {
            // TODO refactor this mess
            if (ls_dbs() == -1) {
                return -1;
            }

            char *db_name = calloc(FILENAME_MAX, sizeof(char));
            if (get_db_name(db_name) == -1) {
                free(db_name);
                return -1;
            }

            if (access(db_name, F_OK) == -1) {
                free(db_name);
                return -1;
            }

            int ent_num = get_db_ent_num(db_name);
            if (ent_num == -1) {
                free(db_name);
                return -1;
            } else if (ent_num == 0) {
                puts("there are no entries in the database");
                free(db_name);
                return 0;
            }

            db_ent **ents = alloc_db_ents(ent_num);
            if (ents == NULL) {
                return -1;
            }

            // scan database, store entries in array
            if (scan_db(db_name, db_ent_scanf_csv, ents, ent_num) == -1) {
                free_db_ents(ents, ent_num);
                free(db_name);
                return -1;
            }

            // get chosen entry member
            printf("%s", db_ent_key_ls_toml);
            enum db_ent_key chos_ent_key = get_db_ent_key();
            if (chos_ent_key == NO_ENT_KEY) {
                free_db_ents(ents, ent_num);
                free(db_name);
                return -1;
            }

            // get sorting order
            enum db_sort_ord chos_sort_ord = get_sort_ord();
            if (chos_sort_ord == NO_SORT_ORD) {
                free_db_ents(ents, ent_num);
                free(db_name);
                return -1;
            }

            // sort array of entries
            sort_db_ents(ents, ent_num, chos_ent_key, chos_sort_ord);

            // erase database
            if (erase_db(db_name) == -1) {
                free_db_ents(ents, ent_num);
                free(db_name);
                return -1;
            }

            // write sorted entries to database
            for (int i = 0; i < ent_num; ++i) {
                if (append_db_ent(db_name, ents[i]) == -1) {
                    free_db_ents(ents, ent_num);
                    free(db_name);
                    return -1;
                }
            }
            printf("database %s has been sorted", db_name);
            free_db_ents(ents, ent_num);
            free(db_name);
            return 0;
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