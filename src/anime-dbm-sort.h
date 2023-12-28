#pragma once

#include "anime-dbm-rec.h"

// DESCRIPTION
//      number of sorting orders
#define sort_ord_num 2

// DESCRIPTION
//      enumeration of sorting orders
enum sort_ord {
    ASC,
    DESC,
    NO_SORT_ORD
};

// DESCRIPTION
//      a lookup table of compar functions for qsort
//      first index is mapped to enum rec_key and second is mapped to sort_ord
extern int (*rec_key_compars[rec_key_num][sort_ord_num])(const void *, const void *);

// DESCRIPTION
//      gets a sort_ord
// ERRORS
//      sets errno to EINVAL and returns NO_SORT_ORD if input is not a sort_ord
enum sort_ord get_sort_ord();