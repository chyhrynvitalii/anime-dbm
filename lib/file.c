#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

int select_csv(const struct dirent *dir_ent) {
    return ends_w_substr(dir_ent->d_name, ".csv");
}

int ls_select_dirent(char *dname, int select(const struct dirent *)) {
    DIR *dir = opendir(dname);
    if (dir == NULL) {
        return -1;
    }
    struct dirent **dir_ents;
    int csv_ent_num;
    if ((csv_ent_num = scandir(dname, &dir_ents, select, alphasort)) == -1) {
        return -1;
    }
    for (int i = 0; i < csv_ent_num; i++) {
        puts(dir_ents[i]->d_name);
        free(dir_ents[i]);
    }
    closedir(dir);
    free(dir_ents);
    return csv_ent_num;
}