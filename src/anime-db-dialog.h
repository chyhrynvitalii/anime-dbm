#pragma once

enum db_cmnd {
    DB_NO_CMND,
    DB_HELP,
    NEW_DB,
    OPEN_DB,
    SORT_DB,
    DEL_DB,
    QUIT,
};

enum ent_cmnd {
    ENT_NO_CMND,
    ENT_HELP,
    NEW_ENT,
    READ_ENT,
    EDIT_ENT,
    DEL_ENT,
    CLOSE_DB
};

enum db_cmnd get_db_cmnd();
enum ent_cmnd get_ent_cmnd();

void ls_db_cmnds();
void ls_ent_cmnds();

void db_dialog();
void ent_dialog(char *);