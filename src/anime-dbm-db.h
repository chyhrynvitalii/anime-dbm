#pragma once

// DESCRIPTION
//      lists databases in the current directory or tells that there are no databases
// RETURN VALUES
//      returns -1 on error, 0 on success
int ls_dbs();

// DESCRIPTION
//      gets database name, stores it in db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
int get_db_name(char *db_name);

// DESCRIPTION
//      creates a new database which name is pointed to by db_name
// RETURN VALUES
//      returns -1 on error, 0 on success
int new_db(const char *db_name);