#pragma once

#define ent_key_num 4
#define sort_ord_num 2

// list of database entry keys
enum db_ent_key {
    TITLE,
    STATUS,
    SCORE,
    PROG,
    NO_ENT_KEY
};

// list of sorting orders
enum db_sort_ord {
    ASC,
    DESC,
    NO_SORT_ORD
};

// database entry structure
typedef struct {
    char *title;
    char *status;
    float score;
    unsigned prog;
} db_ent;

// log entry structure
typedef struct {
    char *db_name;
    enum db_sort_ord sort_ord;
    enum db_ent_key ent_key;
} log_ent;

// log file name
extern const char *log_name;

// length of database entry title
extern const size_t title_len;

// database entry print / scan formats
extern const char *db_ent_printf_csv, *db_ent_printf_toml, *db_ent_scanf_csv, *db_ent_title_printf_toml;

// database entry key list format
extern const char *db_ent_key_ls_toml;

// allocates memory for a database entry
// returns NULL on error, pointer to an allocated entry on success
db_ent *alloc_db_ent();

// deallocates memory pointed by a database entry pointer
void free_db_ent(db_ent *ent);

// get a database entry title, store it in a pointer
// returns -1 on error, 0 on success
int get_db_ent_title(db_ent *ent);

// get a database entry status, store it in a pointer
// returns -1 on error, 0 on success
int get_db_ent_status(db_ent *);

// get a database entry score, store it in a pointer
// returns -1 on error, 0 on success
int get_db_ent_score(db_ent *);

// get a database entry progress, store it in a pointer
// returns -1 on error, 0 on success
int get_db_ent_prog(db_ent *);

// get a database entry, store it in a pointer
// returns -1 on error, 0 on success
int get_db_ent(db_ent *);

// appends a database entry to a database
// returns -1 on error, 0 on success
int append_db_ent(char *, db_ent *);

// calculates a number of database entries in a database
// returns -1 on error, number of entries in database on success
int get_db_ent_num(char *);

// scans a database for the given number of database entries, store result in a database entry pointer
// returns -1 on error, 0 on success
int scan_db(char *, const char *, db_ent **, int);

// prints out a database entry to stdin with the given format
void printf_db_ent(db_ent *, const char *);

// prints out titles of database entries to stdin with the given format
void ls_titles(db_ent **, const char *, int );

// searches through an array of database entry pointers for a matching title
// returns a pointer to a database entry with a matching title, NULL if no matches have been found
db_ent *get_match_title_db_ent(db_ent **, const char *, int );

// erases a database
// returns -1 on error, 0 on success
int erase_db(char *);

// get sorting order
// returns NO_SORT_ORD and sets errno on error, sorting order on success
enum db_sort_ord get_sort_ord();

// get entry member
// returns NO_ENT_KEY and sets errno on error, entry member on success
enum db_ent_key get_db_ent_key();

// sorts an array of entries
// returns -1 on error, 0 on success
void sort_db_ents(db_ent **, int, enum db_ent_key, enum db_sort_ord);

// free an array of entries
void free_db_ents(db_ent **, int);

// allocate an array of entries
// returns NULL on error, pointer to an allocated array of entries on SUCCESS
db_ent **alloc_db_ents(int);

// DESCRIPTION
//      lists databases in the current directory or tells that there are no databases
// RETURN VALUES
//      returns -1 on error, 0 on success
int ls_dbs();

// DESCRIPTION
//      gets a database name, writes it to the given pointer
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_db_name(char *db_name);

// DESCRIPTION
//      creates a new database with the given name
// RETURN VALUES
//      returns -1 on error, 0 on success
int new_db(const char *db_name);

// DESCRIPTION
//      if a database with the given name exists, sets close_db_flag to 0
//      calls db_ent_dialog in a loop until close_db_flag is set to 1
//      in case db_ent_dialog returns -1, calls perror
// RETURN VALUES
//      returns -1 on error, 0 on success
int open_db(const char *db_name);