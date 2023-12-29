#pragma once

#include "anime-dbm-sort.h"
#include "anime-dbm-rec.h"

// DESCRIPTION
//      config record structure
typedef struct {
    char *db_name;
    enum rec_key rec_key;
    enum sort_ord sort_ord;
} conf_rec;

// DESCRIPTION
//      config name
extern const char *conf_name;

// DESCRIPTION
//      returns the number of databases in the current directory
int get_db_num();

// DESCRIPTION
//      returns a pointer to memory allocated for a config record with all fields nullified
conf_rec *calloc_conf_rec();

// DESCRIPTION
//      deallocates memory allocated for a config record pointed to by conf_rec
void free_conf_rec(conf_rec *conf_rec);

// DESCRIPTION
//      returns a pointer to memory allocated for db_num config record pointers with all fields nullified
conf_rec **calloc_conf_recs(int db_num);

// DESCRIPTION
//      deallocates memory allocated for db_num config record pointers pointed to by conf_recs
void free_conf_recs(conf_rec **conf_recs, int db_num);

// DESCRIPTION
//      appends a config record pointed to by conf_rec to the config
// RETURN VALUES
//      returns -1 on error, 0 on success
int append_conf_rec(conf_rec *conf_rec);

// DESCRIPTION
//      writes db_num config records pointed to by conf_recs to config
// RETURN VALUES
//      returns -1 on error, 0 on success
int write_conf_recs(conf_rec **conf_recs, int db_num);

// DESCRIPTION
//      scans config for db_num config records, stores scanned records in conf_recs
// RETURN VALUES
//      returns -1 on error, 0 on success
int scan_conf_recs(conf_rec **conf_recs, int db_num);

// DESCRIPTION
//      gets a config record from db_num config records pointed to by conf_recs with the same db_name as db_name
// RETURN VALUES
//      returns the matching record or NULL if a matching record has not been found
conf_rec *get_target_conf_rec(conf_rec **conf_recs, int db_num, char *db_name);

// DESCRIPTION
//      creates a new config
// RETURN VALUES
//      returns -1 on error, 0 on success
int new_conf();

// DESCRIPTION
//      updates database db_name config record
// RETURN VALUES
//      returns -1 on error, 0 on success
int update_conf(char *db_name, enum rec_key rec_key, enum sort_ord sort_ord);