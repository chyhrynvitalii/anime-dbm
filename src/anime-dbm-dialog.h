#pragma once

// list of database commands
enum db_cmnd {
    DB_NO_CMND,
    DB_HELP,
    NEW_DB,
    OPEN_DB,
    SORT_DB,
    DEL_DB,
    CLOSE_DBM,
};

// list of entry commands
enum ent_cmnd {
    ENT_NO_CMND,
    ENT_HELP,
    NEW_ENT,
    READ_ENT,
    EDIT_ENT,
    DEL_ENT,
    CLOSE_DB
};

// global variable that indicates if a command to close the database has been called
// is set by db_ent_dialog
int close_db_flag;

// global variable that indicates if a command to close the application has been called
// is set by db_dialog
int close_dbm_flag;

// get database command
// returns DB_NO_CMND on error, a database command from the list of database commands on success
enum db_cmnd get_db_cmnd();

// get entry command
// returns ENT_NO_CMND on error, an entry command from the list of entry commands on success
enum ent_cmnd get_ent_cmnd();

// list database commands
// returns -1 on error, 0 on success
int ls_db_cmnds();

// list entry commands
// returns -1 on error, 0 on success
int ls_ent_cmnds();

// database dialog
// returns -1 on error, 0 on success
int db_dialog();

// entry dialog
// returns -1 on error, 0 on success
int db_ent_dialog(char *);