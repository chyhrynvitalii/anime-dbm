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

// length of database entry title
extern const size_t title_len;
// database entry print / scan formats
extern const char *ent_printf_csv, *ent_printf_toml, *ent_scanf_csv, *title_printf_toml;
// database entry key list format
extern const char *ent_key_ls_toml;

// get a database name
// returns -1 on error, 0 on success
int get_db_name(char *);

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

// scans a database for a given number of database entries, store result in a database entry pointer
// returns -1 on error, 0 on success
int scan_db(char *, const char *, db_ent **, int);

// prints out a database entry to stdin with a given format
void printf_db_ent(db_ent *, const char *);

// prints out titles of database entries to stdin with a given format
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