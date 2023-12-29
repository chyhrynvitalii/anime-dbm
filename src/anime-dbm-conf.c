#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anime-dbm-conf.h"
#include "file.h"

// initializing config name

const char *conf_name = "anime-dbm.conf";

// DESCRIPTION
//      config record csv print format
const char *conf_rec_printf_csv = "\"%s\",%i,%i\n";

// DESCRIPTION
//      config record csv scan format
const char *conf_rec_scanf_csv = "\"%[^\"]\",%i,%i%*c";

int get_db_num() {
    return get_select_dir_ent_num(cur_dir, select_csv);
}

conf_rec *calloc_conf_rec() {
    conf_rec *conf_rec_ptr = calloc(1, sizeof(conf_rec));
    conf_rec_ptr->db_name = calloc(FILENAME_MAX, sizeof(char));
    conf_rec_ptr->rec_key = NO_REC_KEY;
    conf_rec_ptr->sort_ord = NO_SORT_ORD;
    return conf_rec_ptr;
}

void free_conf_rec(conf_rec *conf_rec) {
    if (conf_rec != NULL) {
        free(conf_rec->db_name);
        free(conf_rec);
    }
}

conf_rec **calloc_conf_recs(int db_num) {
    conf_rec **conf_recs = calloc(db_num, sizeof(conf_rec *));
    for (int i = 0; i < db_num; ++i) {
        conf_recs[i] = calloc_conf_rec();
    }
    return conf_recs;
}

void free_conf_recs(conf_rec **conf_recs, int db_num) {
    for (int i = 0; i < db_num; ++i) {
        free_conf_rec(conf_recs[i]);
    }
    free(conf_recs);
}

int append_conf_rec(conf_rec *conf_rec) {
    FILE *db = fopen(conf_name, "a");
    if (db == NULL) {
        return -1;
    } else {
        fprintf(db, conf_rec_printf_csv, conf_rec->db_name, conf_rec->rec_key, conf_rec->sort_ord);
        fclose(db);
        return 0;
    }
}

int write_conf_recs(conf_rec **conf_recs, int db_num) {
    FILE *conf = fopen(conf_name, "w");
    if (conf == NULL) {
        return -1;
    }

    for (int i = 0; i < db_num; ++i) {
        if (conf_recs[i] != NULL) {
            append_conf_rec(conf_recs[i]);
        }
    }
    return 0;
}

int scan_conf_recs(conf_rec **conf_recs, int db_num) {
    FILE *conf = fopen(conf_name, "r");
    if (conf == NULL) {
        return -1;
    }
    for (int i = 0; i < db_num; ++i) {
        fscanf(conf, conf_rec_scanf_csv, conf_recs[i]->db_name, &conf_recs[i]->rec_key, &conf_recs[i]->sort_ord);
    }
    fclose(conf);
    return 0;
}

conf_rec *get_target_conf_rec(conf_rec **conf_recs, int db_num, char *db_name) {
    for (int i = 0; i < db_num; ++i) {
        if (strcmp(conf_recs[i]->db_name, db_name) == 0) {
            return conf_recs[i];
        }
    }
    return NULL;
}

int new_conf() {
    FILE *conf = fopen(conf_name, "wx");
    if (conf == NULL) {
        return -1;
    } else {
        printf("config file %s has been created\n", conf_name);
        fclose(conf);
        return 0;
    }
}

int update_conf(char *db_name, enum rec_key rec_key, enum sort_ord sort_ord) {
    int db_num = get_db_num();
    conf_rec **conf_recs = calloc_conf_recs(db_num);
    if (scan_conf_recs(conf_recs, db_num) == -1) {
        return -1;
    }

    conf_rec *target_conf_rec = get_target_conf_rec(conf_recs, db_num, db_name);
    if (target_conf_rec == NULL) {
        return -1;
    }

    target_conf_rec->rec_key = rec_key;
    target_conf_rec->sort_ord = sort_ord;

    if (write_conf_recs(conf_recs, db_num) == -1) {
        return -1;
    } else {
        return 0;
    }
}