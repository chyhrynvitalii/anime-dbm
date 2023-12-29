#pragma once

// DESCRIPTION
//      enumerated list of commands for managing databases
enum db_cmnd {
    DB_HELP,
    NEW_DB,
    OPEN_DB,
    DEL_DB,
    CLOSE_DBM,
    NO_DB_CMND
};

// DESCRIPTION
//      enumerated list of commands for managing database records
enum rec_cmnd {
    REC_HELP,
    NEW_REC,
    READ_REC,
    EDIT_REC,
    SORT_RECS,
    DEL_REC,
    CLOSE_DB,
    NO_REC_CMND
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
//      gets a database command and calls it
// RETURN VALUES
//      returns -1 on error, 0 on success
int db_dialog();

// DESCRIPTION
//      gets a record command and calls it
// RETURN VALUES
//      returns -1 on error, 0 on success
int db_rec_dialog(char *db_name);