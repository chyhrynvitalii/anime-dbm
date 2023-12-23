#pragma once

#include "dirent.h"
#include <stdlib.h>

int get_fsize(char *, size_t *);
int select_csv(const struct dirent *);
int ls_select_dirent(char *, int (const struct dirent *));