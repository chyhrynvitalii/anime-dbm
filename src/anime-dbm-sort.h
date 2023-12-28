#pragma once

#define rec_key_num 4
#define sort_ord_num 2

// DESCRIPTION
//      a lookup table of compar functions for qsort
//      row is rec_key and column is sort_ord
extern int (*compar_rec_key_lut[rec_key_num][sort_ord_num])(const void *, const void *);