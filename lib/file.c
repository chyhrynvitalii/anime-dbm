#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "str.h"

int get_fsize(char *fname, size_t *size_buf) {
    struct stat sb;
    if (stat(fname, &sb) == -1) {
        return -1;
    } else {
        *size_buf =  sb.st_size;
        return 0;
    }
}

int ls_fnames_w_fext(char *dname, char *fext) {
    DIR *cur_dir = opendir(dname);
    if (cur_dir == NULL) {
        return -1;
    }
    struct dirent *dir_ent;
    while ((dir_ent = readdir(cur_dir)) != NULL) {
        if (ends_w_substr(dir_ent->d_name, fext)) {
            puts(dir_ent->d_name);
        }
    }
    return 0;
}