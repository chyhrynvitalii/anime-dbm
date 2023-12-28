#pragma once

// DESCRIPTION
//      number of record keys
#define rec_key_num 4

// DESCRIPTION
//      enumerated list of record keys
enum rec_key {
    TITLE,
    STATUS,
    SCORE,
    PROG,
    NO_REC_KEY
};

// DESCRIPTION
//      record structure
typedef struct {
    char *title;
    char *status;
    float score;
    unsigned prog;
} rec;

// DESCRIPTION
//      gets a new record and stores it in the database db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
int new_rec(char *db_name);

// DESCRIPTION
//      gets the title of a record from the database db_name
//      then outputs the corresponding record to stdout as toml
// RETURN VALUES
//      returns -1 on error, 0 on success
int read_rec(char *db_name);

// DESCRIPTION
//      gets the title of a record from the database db_name
//      then gets the record key to edit, its new value and stores the edited record in the database
// RETURN VALUES
//      returns -1 on error, 0 on success
int edit_rec(char *db_name);

// DESCRIPTION
//      gets the title of a record from the database db_name
//      then deletes the corresponding record from the database
// RETURN VALUES
//      returns -1 on error, 0 on success
int del_rec(char *db_name);