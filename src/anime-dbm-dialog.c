#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "anime-dbm-dialog.h"
#include "anime-dbm-engine.h"
#include "file.h"
#include "get.h"

const size_t cmnd_len = 6;

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

int ent_dialog(char *dbname) {
    // get entry command
    enum ent_cmnd cmnd = get_ent_cmnd();

    switch (cmnd) {
        case DB_HELP: {
            // list entry commands
            if (ls_ent_cmnds() == -1) {
                return -1;
            }

            return 0;
        }
        case NEW_ENT: {
            ent *ent = alloc_ent();
            if (ent == NULL) {
                return -1;
            }
            if (get_ent(ent) == -1) {
                free_ent(ent);
                return -1;
            }
            if (append_ent(dbname, ent) == -1) {
                free_ent(ent);
                return -1;
            }
            printf("entry %s has been created", ent->title);
            free_ent(ent);
            return 0;
        }
        case READ_ENT: {
            int ent_num = get_ent_num(dbname);
            if (ent_num == -1) {
                return -1;
            } else if (ent_num == 0) {
                puts("there are no entries in the database");
                return 0;
            }
            ent **ents = alloc_ents(ent_num);
            if (ents == NULL) {
                return -1;
            }
            if (scan_db(dbname, ent_scanf_csv, ents, ent_num) == -1) {
                free_ents(ents, ent_num);
                return -1;
            }
            if (ls_titles(ents, title_printf_toml, ent_num) == -1) {
                free_ents(ents, ent_num);
                return -1;
            }
            char *ent_title = calloc(title_len, sizeof(char));
            if (get_str(title_len, "title: ", ent_title) == -1) {
                free_ents(ents, ent_num);
                free(ent_title);
                return -1;
            }
            ent *chos_ent;
            if ((chos_ent = get_ent_w_match_title(ents, ent_title, ent_num)) == NULL) {
                free_ents(ents, ent_num);
                free(ent_title);
                errno = EINVAL;
                return -1;
            }
            if (printf_ent(chos_ent, ent_printf_toml) == -1) {
                free_ents(ents, ent_num);
                free(ent_title);
                return -1;
            }
            for (int i = 0; i < ent_num; ++i) {
                free_ent(ents[i]);
            }
            free(ents);
            free(ent_title);
            return 0;
        }
        case EDIT_ENT: {
            int ent_num = get_ent_num(dbname);
            if (ent_num == -1) {
                return -1;
            } else if (ent_num == 0) {
                puts("there are no entries in the database");
                return 0;
            }
            ent **ents = alloc_ents(ent_num);
            if (ents == NULL) {
                return -1;
            }
            if (scan_db(dbname, ent_scanf_csv, ents, ent_num) == -1) {
                return -1;
            }
            if (ls_titles(ents, title_printf_toml, ent_num) == -1) {
                free_ents(ents, ent_num);
                return -1;
            }
            char *chosen_ent_title = calloc(title_len, sizeof(char));
            if (get_str(title_len, "title: ", chosen_ent_title) == -1) {
                free_ents(ents, ent_num);
                free(chosen_ent_title);
                return -1;
            }
            ent *chos_ent;
            if ((chos_ent = get_ent_w_match_title(ents, chosen_ent_title, ent_num)) == NULL) {
                free_ents(ents, ent_num);
                free(chosen_ent_title);
                errno = EINVAL;
                return -1;
            }
            printf("%s", ent_memb_ls_toml);
            enum ent_memb chos_ent_memb = get_ent_memb();
            switch (chos_ent_memb) {
                case TITLE: {
                    if (get_ent_title(chos_ent) == -1) {
                        free_ents(ents, ent_num);
                        free(chosen_ent_title);
                        return -1;
                    }
                    break;
                }
                case STATUS: {
                    if (get_ent_status(chos_ent) == -1) {
                        free_ents(ents, ent_num);
                        free(chosen_ent_title);
                        return -1;
                    }
                    break;
                }
                case SCORE: {
                    if (get_ent_score(chos_ent) == -1) {
                        free_ents(ents, ent_num);
                        free(chosen_ent_title);
                        return -1;
                    }
                    break;
                }
                case PROG: {
                    if (get_ent_prog(chos_ent) == -1) {
                        free_ents(ents, ent_num);
                        free(chosen_ent_title);
                        return -1;
                    }
                    break;
                }
                case NO_ENT_MEMB: {
                    free_ents(ents, ent_num);
                    free(chosen_ent_title);
                    return -1;
                }
            }
            if (erase_db(dbname) == -1) {
                free_ents(ents, ent_num);
                free(chosen_ent_title);
                return -1;
            }
            for (int i = 0; i < ent_num; ++i) {
                if (append_ent(dbname, ents[i]) == -1) {
                    free_ents(ents, ent_num);
                    free(chosen_ent_title);
                    return -1;
                }
            }
            printf("entry %s has been edited", chosen_ent_title);
            for (int i = 0; i < ent_num; ++i) {
                free_ent(ents[i]);
            }
            free(ents);
            free(chosen_ent_title);
            return 0;
        }
        case DEL_ENT: {
            int ent_num = get_ent_num(dbname);
            if (ent_num == -1) {
                return -1;
            } else if (ent_num == 0) {
                puts("there are no entries in the database");
                return 0;
            }
            ent **ents = alloc_ents(ent_num);
            if (ents == NULL) {
                return -1;
            }
            if (scan_db(dbname, ent_scanf_csv, ents, ent_num) == -1) {
                free_ents(ents, ent_num);
                return -1;
            }
            if (ls_titles(ents, title_printf_toml, ent_num) == -1) {
                free_ents(ents, ent_num);
                return -1;
            }
            char *chos_ent_title = calloc(title_len, sizeof(char));
            if (get_str(title_len, "title: ", chos_ent_title) == -1) {
                free_ents(ents, ent_num);
                return -1;
            }
            ent *chos_ent;
            if ((chos_ent = get_ent_w_match_title(ents, chos_ent_title, ent_num)) == NULL) {
                free_ents(ents, ent_num);
                free(chos_ent_title);
                errno = EINVAL;
                return -1;
            }
            if (erase_db(dbname) == -1) {
                free_ents(ents, ent_num);
                free(chos_ent_title);
                return -1;
            }
            for (int i = 0; i < ent_num; ++i) {
                if (ents[i] != chos_ent) {
                    if (append_ent(dbname, ents[i]) == -1) {
                        free_ents(ents, ent_num);
                        free(chos_ent_title);
                        return -1;
                    }
                }
            }
            printf("entry %s has been deleted", chos_ent_title);
            for (int i = 0; i < ent_num; ++i) {
                free_ent(ents[i]);
            }
            free(ents);
            free(chos_ent_title);
            return 0;
        }
        case CLOSE_DB: {
            close_db_flag = 1;

            return 0;
        }
        case ENT_NO_CMND: {
            return -1;
        }
    }
}

int db_dialog() {
    // get db command
    enum db_cmnd cmnd = get_db_cmnd();

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
            char *dbname = calloc(FILENAME_MAX, sizeof(char));
            if (get_dbname(dbname) == -1) {
                return -1;
            }
            // create database
            FILE *db = fopen(dbname, "wx");
            if (db == NULL) {
                return -1;
            } else {
                printf("database %s has been created\n", dbname);
            }
            fclose(db);
            free(dbname);
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
            char *dbname = calloc(FILENAME_MAX, sizeof(char));
            if (get_dbname(dbname) == -1) {
                return -1;
            }
            // check if database with such name exists
            if (access(dbname, F_OK) == -1) {
                return -1;
            }
            // open database
            close_db_flag = 0;
            printf("database %s has been opened\n", dbname);
            do {
                if (ent_dialog(dbname) == -1) {
                    perror("error");
                }
            } while (close_db_flag != 1);
            printf("database %s has been closed\n", dbname);
            free(dbname);
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
            char *dbname = calloc(FILENAME_MAX, sizeof(char));
            if (get_dbname(dbname) == -1) {
                free(dbname);
                return -1;
            }
            // check if database with such name exists
            if (access(dbname, F_OK) == -1) {
                free(dbname);
                return -1;
            }
            // get number of entries
            int ent_num = get_ent_num(dbname);
            if (ent_num == -1) {
                free(dbname);
                return -1;
            } else if (ent_num == 0) {
                puts("there are no entries in the database");
                free(dbname);
                return 0;
            }
            // allocate memory for an array of entries
            ent **ents = alloc_ents(ent_num);
            if (ents == NULL) {
                return -1;
            }
            // scan database, store entries in the array
            if (scan_db(dbname, ent_scanf_csv, ents, ent_num) == -1) {
                free_ents(ents, ent_num);
                free(dbname);
                return -1;
            }
            // get chosen entry member
            printf("%s", ent_memb_ls_toml);
            enum ent_memb chos_ent_memb = get_ent_memb();
            if (chos_ent_memb == NO_ENT_MEMB) {
                free_ents(ents, ent_num);
                free(dbname);
                return -1;
            }
            // get sorting order
            enum sort_ord chos_sort_ord = get_sort_ord();
            if (chos_sort_ord == NO_SORT_ORD) {
                free_ents(ents, ent_num);
                free(dbname);
                return -1;
            }
            // sort array of entries
            if (sort_ents(ents, ent_num, chos_ent_memb, chos_sort_ord) == -1) {
                free_ents(ents, ent_num);
                free(dbname);
                return -1;
            }
            // erase database
            if (erase_db(dbname) == -1) {
                free_ents(ents, ent_num);
                free(dbname);
                return -1;
            }
            // write sorted entries to database
            for (int i = 0; i < ent_num; ++i) {
                if (append_ent(dbname, ents[i]) == -1) {
                    free_ents(ents, ent_num);
                    free(dbname);
                    return -1;
                }
            }
            printf("database %s has been sorted", dbname);
            free_ents(ents, ent_num);
            free(dbname);
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
            char *dbname = calloc(FILENAME_MAX, sizeof(char));
            if (get_dbname(dbname) == -1) {
                return -1;
            }
            // check if database with such name exists
            if (access(dbname, F_OK) == -1) {
                return -1;
            }
            // delete database
            if (remove(dbname) == -1) {
                return -1;
            }
            printf("database %s has been deleted", dbname);
            free(dbname);
            return 0;
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