#pragma once

#include "dirent.h"
#include <stdlib.h>

// selects csv files for scandir
int select_csv(const struct dirent *);

// lists files filtered by a select function in a given directory
int ls_select_dir_ent(char *dir_name, int (*select)(const struct dirent *));