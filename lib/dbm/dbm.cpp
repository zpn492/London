
/* ******************************************************* */
/* PROTECTED ********************************************* */
/* ******************************************************* */
#include "dbm.h"

using namespace filehandler;

bool Dbm::t_execute(const char *db_name, std::string query, int callback)
    {
    /* include headers */
    qr.active = 1;
	qr.result = "";
    error = "";
    
    /* Open db connection */
    sqlite3_open(db_name, &db);
    
    /* Execute query */
    switch(callback)
        {
        case 0:
        rc = sqlite3_exec(db, query.c_str(), NULL, 0, &err);
        break;
        case 1:
        const char *data;
        rc = sqlite3_exec(db, query.c_str(), t_select_statement, (void*)data, &err);
        break;
        default:
        rc = sqlite3_exec(db, query.c_str(), NULL, 0, &err);
        };
    
    // Upon query error
    if(rc != SQLITE_OK)
        {
        error = std::string(err);
        write_file("exe_log.txt", std::string(err));
        sqlite3_free(err);
        write_file("query_log.txt", query);
        return false;
        }  
    
    sqlite3_close(db);

    result = qr.result;
    
    qr.result = "";

    return true;
    };

int Dbm::t_select_statement(void *NotUsed, int argc, char **argv, char **azColName)
    {
    int i;
    if(qr.active)
        {
        for(i = 0; i < argc; i++) // Add headers
            {
			if(i == argc-1)
				qr.result += std::string(azColName[i]);
			else
				qr.result += std::string(azColName[i]) + ";";
            }
		qr.result += "\n";
        }
	
    qr.active = 0; // Add headers once

    for(i = 0; i < argc; i++)
		{
		std::string s(argv[i]);
		if(i == argc-1)
			qr.result += s;
		else
			qr.result += s + ";";
		}
	qr.result += '\n';
    return 0;
    };

std::string Dbm::t_result()
    {
    return result;
    };

std::string Dbm::t_error()
    {
    return error;
    };