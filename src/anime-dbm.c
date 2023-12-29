#include <stdio.h>
#include <stdlib.h>

#include "anime-dbm-dialog.h"
#include "anime-dbm-conf.h"

int main() {
    new_conf();

    do {
        if (db_dialog() == -1) {
            perror("error");
        }
    } while (close_dbm_flag != 1);

    return EXIT_SUCCESS;
}