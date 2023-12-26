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
//      enumerated list of commands for managing database entries
enum db_ent_cmnd {
    DB_ENT_NO_CMND,
    ENT_HELP,
    NEW_ENT,
    READ_ENT,
    EDIT_ENT,
    DEL_ENT,
    CLOSE_DB
};

// DESCRIPTION
//      indicates if a command to close the database has been called
//      is set by db_ent_dialog
int close_db_flag;

// DESCRIPTION
//      indicates if a command to close the application has been called
//      is set by db_dialog
int close_dbm_flag;

// DESCRIPTION
//      lists database commands
// RETURN VALUES
//      returns -1 on error, 0 on success
void ls_db_cmnds();

// DESCRIPTION
//      list database entry commands
// RETURN VALUES
//      returns -1 on error, 0 on success
void ls_db_ent_cmnds();

// DESCRIPTION
//      dialogue menu for managing databases
// RETURN VALUES
//      returns -1 on error, 0 on success
int db_dialog();

// DESCRIPTION
//      dialogue menu for managing entries of a database which name is pointed to by db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
int db_ent_dialog(char *db_name);