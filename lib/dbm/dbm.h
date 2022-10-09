#ifndef __DBM__
#define __DBM__

#include <string>
#include <vector>
#include <algorithm>
#include "../../ext/sqlite/sqlite3.h"
#include "../logger/logger.h"
#include "../filehandler/filehandler.h"

/* ******************************************************* */
/* Class CsvDb ******************************************* */
/* ******************************************************* */

class Dbm
    {
    public:
        bool t_execute(const char *db_name, std::string query, int callback);

        static int t_select_statement(void *NotUsed, int argc, char **argv, char **azColName);

        std::string t_result();
        std::string t_error();
    private:
        sqlite3 *db;
        char *err;
        int rc;
        std::string result;
        std::string error;
    };

/* ******************************************************* */
/* Structs *********************************************** */
/* ******************************************************* */
struct QueryResult 
    {
    int active;
	std::string result;
    } static qr;

#endif