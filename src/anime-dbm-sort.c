#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "anime-dbm-rec.h"
#include "anime-dbm-sort.h"
#include "get.h"

// DESCRIPTION
//      max length of sorting order as a string
const size_t sort_ord_len = 10;

int compar_title_asc(const void *rec1, const void *rec2) {
    return strcmp((*(rec **)rec1)->title, (*(rec **)rec2)->title);
}

int compar_status_asc(const void *rec1, const void *rec2) {
    return strcmp((*(rec **)rec1)->status, (*(rec **)rec2)->status);
}

int compar_score_asc(const void *rec1, const void *rec2) {
    return (*(rec **)rec1)->score > (*(rec **)rec2)->score ? 1 : -1;
}

int compar_prog_asc(const void *rec1, const void *rec2) {
    return (int)((*(rec **)rec1)->prog - (*(rec **)rec2)->prog);
}

int compar_title_desc(const void *rec1, const void *rec2) {
    return compar_title_asc(rec2, rec1);
}

int compar_status_desc(const void *rec1, const void *rec2) {
    return compar_status_asc(rec2, rec1);
}

int compar_score_desc(const void *rec1, const void *rec2) {
    return compar_score_asc(rec2, rec1);
}

int compar_prog_desc(const void *rec1, const void *rec2) {
    return compar_prog_asc(rec2, rec1);
}

int (*rec_key_compars[rec_key_num][sort_ord_num])(const void *, const void *) =
        {{compar_title_asc, compar_title_desc},
         {compar_status_asc, compar_status_desc},
         {compar_score_asc, compar_score_desc},
         {compar_prog_asc, compar_prog_desc}};


enum sort_ord get_sort_ord() {
    char *sort_ord = calloc(sort_ord_len, sizeof(char));
    if (get_str(sort_ord_len, "sorting order (ascending or descending): ", sort_ord) == -1) {
        return -1;
    }

    if (strcasecmp(sort_ord, "ascending") == 0) {
        free(sort_ord);
        return ASC;
    } else if (strcasecmp(sort_ord, "descending") == 0) {
        free(sort_ord);
        return DESC;
    } else {
        free(sort_ord);
        errno = EINVAL;
        return NO_SORT_ORD;
    }
}