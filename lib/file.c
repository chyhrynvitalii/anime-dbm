#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

int select_csv(const struct dirent *dir_ent) {
    return ends_substr(dir_ent->d_name, ".csv");
}

int ls_select_dir_ent(char *dir_name, int select(const struct dirent *)) {
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        return -1;
    }

    struct dirent **dir_ents;
    int csv_num;
    if ((csv_num = scandir(dir_name, &dir_ents, select, alphasort)) == -1) {
        return -1;
    }

    for (int i = 0; i < csv_num; i++) {
        puts(dir_ents[i]->d_name);
        free(dir_ents[i]);
    }

    closedir(dir);
    free(dir_ents);
    return csv_num;
}