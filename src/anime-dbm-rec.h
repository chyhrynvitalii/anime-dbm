#pragma once

#define rec_key_num 4
#define sort_ord_num 2

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
//      enumerated list of sorting orders
enum sort_ord {
    ASC,
    DESC,
    NO_SORT_ORD
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
//      record title length
extern const size_t title_len;

// DESCRIPTION
//      allocates memory for a record structure
//      returns a pointer to allocated memory
rec *alloc_rec();

// DESCRIPTION
//     deallocates memory allocated for a record structure pointed to by rec
void free_rec(rec *rec);

// DESCRIPTION
//      allocates memory for an array of record structures of size rec_num
//      returns a pointer to allocated memory
rec **alloc_recs(int rec_num);

// DESCRIPTION
//     deallocates memory allocated for an array of record structures of size rec_num pointed to by recs
void free_recs(rec **recs, int rec_num);

// DESCRIPTION
//      gets rec->title, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec_title(rec *rec);

// DESCRIPTION
//      gets rec->status, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec_status(rec *rec);

// DESCRIPTION
//      gets rec->score, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
// ERRORS
//      sets errno to EDOM if input does not belong to the domain of score
int get_rec_score(rec *rec);

// DESCRIPTION
//      gets rec->prog, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec_prog(rec *rec);

// DESCRIPTION
//      gets rec, stores it in rec
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec(rec *rec);

// DESCRIPTION
//      appends a record pointed to by rec to a database which name is pointed to by db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
int append_rec(const char *db_name, rec *rec);

// DESCRIPTION
//      writes a record array of size rec_num pointed to by recs to database which name is pointed to by db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
int write_recs(const char *db_name, rec **recs, int rec_num);

// DESCRIPTION
//      scans a database which name is pointed to by db_name, stores scanned records in recs
// RETURN VALUES
//      returns -1 on error, 0 on success
int scan_recs(char *db_name, rec **recs, int rec_num);

// DESCRIPTION
//      lists titles of records from a record array of size rec_num pointed to by recs
void ls_rec_titles(rec **recs, int rec_num);

// DESCRIPTION
//      gets record title as string, stores it in rec_title_buf
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_rec_title_str(char *rec_title_buf);

// DESCRIPTION
//      searches through recs for a record with the same title as rec_title
//      returns a pointer to a matching record or NULL
rec *get_match_title_rec(rec **recs, const char *rec_title, int rec_num);

// DESCRIPTION
//      gets a record title, searches through recs for a record with a matching title
//      returns a pointer to a record with a matching title or NULL
rec *get_target_rec(rec **recs, int rec_num);

// DESCRIPTION
//      prints out a record pointed to by rec to stdin as toml
void put_rec(rec *rec);

// DESCRIPTION
//      gets rec_key
// RETURN VALUES
//      returns NO_REC_KEY on error, an actual rec_key on success
// ERRORS
//      sets errno to EINVAL if input is not a rec_key
enum rec_key get_rec_key();

// DESCRIPTION
//      calls get_rec_key to edit a record pointed to by rec where key corresponds to key rec_key
// RETURN VALUES
//      returns -1 on error, 0 success
int edit_rec_key(rec *rec, enum rec_key rec_key);

// DESCRIPTION
//      gets sort_ord
// RETURN VALUES
//      returns NO_SORT_ORD on error, an actual sort_ord on success
// ERRORS
//      sets errno to EINVAL if input is not a sort_ord
enum sort_ord get_sort_ord();

// DESCRIPTION
//      a lookup table of compar functions for qsort
//      row is a rec_key and column is a sort_ord
extern int (*compar_rec_key_lut[rec_key_num][sort_ord_num])(const void *, const void *);