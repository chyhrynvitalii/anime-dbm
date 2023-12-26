#pragma once

#include <dirent.h>
#include <stdlib.h>

// DESCRIPTION
//      selects csv files for scandir
// RETURN VALUES
//      returns a non-zero integer if dir_ent is a csv file
int select_csv(const struct dirent *dir_ent);

// DESCRIPTION
//      lists dir_name directory entries filtered by the select function which is fed to scandir
// RETURN VALUES
//      returns -1 on error, number of directory entries listed on success
int ls_select_dir_ent(char *dir_name, int (*select)(const struct dirent *));