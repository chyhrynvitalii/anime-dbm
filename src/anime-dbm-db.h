#pragma once

// DESCRIPTION
//      lists databases in the current directory or tells that there are no databases
// RETURN VALUES
//      returns number of databases listed
int ls_dbs();

// DESCRIPTION
//      gets database name, stores it in db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_db_name(char *db_name);

// DESCRIPTION
//      gets a database name and creates a new database with that name
// RETURN VALUES
//      returns -1 on error, 0 on success
int new_db();

// DESCRIPTION
//      gets the database name and calls db_rec_dialog with it as an argument until close_db_flag is set to 1
//      in case db_rec_dialog returns -1, calls perror, but doesn't terminate the loop
// RETURN VALUES
//      returns -1 on error, 0 on success
int open_db();

// DESCRIPTION
//      gets the database name and sorts it
// RETURN VALUES
//      returns -1 on error, 0 on success
int sort_db();

// DESCRIPTION
//      gets the database name and deletes it
// RETURN VALUES
//      returns -1 on error, 0 on success
int delete_db();