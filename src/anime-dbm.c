#include <stdio.h>
#include <stdlib.h>

#include "anime-dbm-dialogs.h"

int main() {
    close_dbm_flag = 0;

    do {
        if (db_dialog() == -1) {
            perror("error");
        }
    } while (close_dbm_flag != 1);

    return EXIT_SUCCESS;
}