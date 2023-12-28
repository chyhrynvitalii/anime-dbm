#pragma once

// DESCRIPTION
//      enumerated list of commands for managing databases
enum db_cmnd {
    DB_NO_CMND,
    DB_HELP,
    NEW_DB,
    OPEN_DB,
    SORT_DB,
    DEL_DB,
    CLOSE_DBM,
};

// DESCRIPTION
//      enumerated list of commands for managing database records
enum rec_cmnd {
    REC_NO_CMND,
    REC_HELP,
    NEW_REC,
    READ_REC,
    EDIT_REC,
    DEL_REC,
    CLOSE_DB
};

// DESCRIPTION
//      indicates if a command to close the database has been called
//      is set by db_rec_dialog
extern int close_db_flag;

// DESCRIPTION
//      indicates if a command to close the application has been called
//      is set by db_dialog
extern int close_dbm_flag;

// DESCRIPTION
//      dialogue menu for managing databases
// RETURN VALUES
//      returns -1 on error, 0 on success
int db_dialog();

// DESCRIPTION
//      dialogue menu for managing records of a database which name is pointed to by db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
int db_rec_dialog(char *db_name);