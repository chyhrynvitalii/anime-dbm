#pragma once

#define ent_memb_num 4
#define sort_ord_num 2

// list of entry members
enum ent_memb {
    TITLE,
    STATUS,
    SCORE,
    PROG,
    NO_ENT_MEMB
};

// list of sorting order options
enum sort_ord {
    ASC,
    DESC,
    NO_SORT_ORD
};

// entry structure
typedef struct {
    char *title;
    char *status;
    float score;
    unsigned prog;
} ent;

// length of entry title
extern const size_t title_len;
// entry print / scan formats
extern const char *ent_printf_csv, *ent_printf_toml, *ent_scanf_csv, *title_printf_toml;
// entry member list format
extern const char *ent_memb_ls_toml;

// get a database name
// returns -1 on error, 0 on success
int get_dbname(char *);

// allocates memory for an entry
// returns NULL on error, pointer to an allocated entry on success
ent *alloc_ent();

// deallocates memory pointed by an entry pointer
void free_ent(ent *);

// get entry title, store it in a pointer
// returns -1 on error, 0 on success
int get_ent_title(ent *);

// get entry status, store it in a pointer
// returns -1 on error, 0 on success
int get_ent_status(ent *);

// get entry score, store it in a pointer
// returns -1 on error, 0 on success
int get_ent_score(ent *);

// get entry progress, store it in a pointer
// returns -1 on error, 0 on success
int get_ent_prog(ent *);

// get an entry, store it in a pointer
// returns -1 on error, 0 on success
int get_ent(ent *);

// appends an entry to a database
// returns -1 on error, 0 on success
int append_ent(char *, ent *);

// calculates a number of entries in a database
// returns -1 on error, number of entries in database on success
int get_ent_num(char *);

// scans a database for a given number of entries, store result in an entry pointer
// returns -1 on error, 0 on success
int scan_db(char *, const char *, ent **, int);

// prints out an entry to stdin with a given format
// returns -1 on error, 0 on success
int printf_ent(ent *, const char *);

// prints out titles of entries to stdin with a given format
// returns -1 on error, 0 on success
int ls_titles(ent **, const char *, int );

// searches through an array of entry pointers for a matching title
// returns a pointer to an entry with a matching title, NULL if no matches have been found
ent *get_ent_w_match_title(ent **, const char *, int );

// erases a database
// returns -1 on error, 0 on success
int erase_db(char *);

// get sorting order
// returns NO_SORT_ORD and sets errno on error, sorting order on success
enum sort_ord get_sort_ord();

// get entry member
// returns NO_ENT_MEMB and sets errno on error, entry member on success
enum ent_memb get_ent_memb();

// sorts an array of entries
// returns -1 on error, 0 on success
void sort_ents(ent **, int, enum ent_memb, enum sort_ord);

// free an array of entries
void free_ents(ent **, int);

// allocate an array of entries
// returns NULL on error, pointer to an allocated array of entries on SUCCESS
ent **alloc_ents(int);