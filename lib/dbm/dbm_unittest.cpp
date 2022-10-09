#include <assert.h>

#include <stdio.h>
#include "dbm.h"
#include "../filehandler/filehandler.h"

using namespace filehandler;

#define DATABASEFILE "dbm.db"
#define LOG_ERRFILE "err_log.txt"
#define LOG_EXEFILE "exe_log.txt"
#define LOG_QUERYFILE "query_log.txt"
#define OUTFILE "dbm.txt"
#define CREATETABLE "CREATE TABLE \"Obj\" ("\
	"\"Idx\"	INTEGER,"\
	"\"Parent\"	INTEGER,"\
	"\"Name\"	TEXT UNIQUE,"\
	"\"Type\"	INTEGER,"\
	"PRIMARY KEY(\"Idx\" AUTOINCREMENT),"\
	"FOREIGN KEY(\"Parent\") REFERENCES \"Obj\"(\"Idx\"))"
#define INSERTINTOTABLE ""\
    "INSERT INTO Obj (Name, Parent, Type) VALUES "\
    "(\"Bank\", 1, 1),"\
    "(\"CREATE\", 1, 2),"\
    "(\"DELETE\", 1, 2),"\
    "(\"GET\", 1, 2)"

#define SELECTFROMTABLE "SELECT * FROM \"Obj\""

#define SELECTFROMTABLERR "SELECT * FROM \"Obj_t\""

void createDatabaseFile()
    {
    filehandler::create_file(DATABASEFILE);
    };

void deleteDatabaseFileCleanUp()
    {
    filehandler::delete_file(DATABASEFILE);
    filehandler::delete_file(LOG_ERRFILE);
    filehandler::delete_file(LOG_EXEFILE);
    filehandler::delete_file(LOG_QUERYFILE);
    };

void createTable(Dbm &dbm)
    {
    dbm.t_execute(DATABASEFILE, CREATETABLE, 0);
    };

void insertIntoTable(Dbm &dbm)
    {
    dbm.t_execute(DATABASEFILE, INSERTINTOTABLE, 0);
    };

void selectFromTable(Dbm &dbm)
    {
    dbm.t_execute(DATABASEFILE, SELECTFROMTABLE, 1);
    };

void selectFromTableErr(Dbm &dbm)
    {
    dbm.t_execute(DATABASEFILE, SELECTFROMTABLERR, 1);
    };

int main(int argc, char *argv[])
    {
    Dbm dbm;
    
    /* Initialize databasefile, table &
     * content */
    createDatabaseFile();
    createTable(dbm);
    insertIntoTable(dbm);
    
    /* TEST CASE Valid expression */
    selectFromTable(dbm);

    /* TEST CASE Error handling */
    selectFromTableErr(dbm);

    /* cleanup */
    deleteDatabaseFileCleanUp();

    return 0;
    };