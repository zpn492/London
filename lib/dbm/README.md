# Database management

This module enables the sqlite3 engine produced back of 2001 September 15.

The main purpose is to handle compiling and implementing of the engine into 
other modules.

## Use case
Access a database

### User story - Valid expression
Given an object Dbm dbm & a database DataIsHere.db, with a table Obj \
When a query against DataIsHere.db returns a result \
Then the object dbm: 
> contains the result as a std::string \
> that result can be accesed through dbm.t_result()

```c++
#include <stdio.h>
#include "dbm.h"
using namespace filehandler;
#define DATABASEFILE "DataIsHere.db"
#define QUERY "SELECT * FROM Obj"
int main(int argc, char *argv[])
    {    
    // Module
    Dbm dbm;

    /* Request against the database
     *
     * const char* : @filepath for database
     * std::string : @query agains the database
     * int         : @callback  
     *** 0 : upon insert, update, delete 
     *** 1 : upon select
     */
    dbm.t_execute(DATABASEFILE, QUERY, 1);

    /* Retrieve result & print
     * 
     */
    std::cout << dbm.t_result().c_str() << std::endl;
    
    return 0;
    }
```

### User story - Error in expression
Given an object Dbm dbm & a database DataIsHere.db, with a table Obj \
When a query against DataIsHere.db returns an error \
Then the object dbm: 
> contains the error as a std::string \
> that error can be accesed through dbm.t_error()
```c++
#include <stdio.h>
#include "dbm.h"
using namespace filehandler;
#define DATABASEFILE "DataIsHere.db"
#define QUERY "SELECT * FROM Ob_t"
int main(int argc, char *argv[])
    {   
    // Module
    Dbm dbm;

    /* Request against the database
     *
     * const char* : @filepath for database
     * std::string : @query agains the database
     * int         : @callback  
     *** 0 : upon insert, update, delete 
     *** 1 : upon select
     */
    dbm.t_execute(DATABASEFILE, QUERY, 1);

    /* Retrieve error & print
     * 
     */
    std::cout << dbm.t_error().c_str() << std::endl;

    return 0;
    }
```

## Dependencies
Module      | Path
----------- | ----
SQLLITE3    | ../../ext/sqlite/ 
Filehandler | ../filehandler/
Logger      | ../logger/
