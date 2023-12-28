#include <string.h>

#include "anime-dbm-rec.h"
#include "anime-dbm-sort.h"

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

int (*compar_rec_key_lut[rec_key_num][sort_ord_num])(const void *, const void *) =
        {{compar_title_asc, compar_title_desc},
         {compar_status_asc, compar_status_desc},
         {compar_score_asc, compar_score_desc},
         {compar_prog_asc, compar_prog_desc}};