/* Handles all standard includes */
#include "SimpleJust.h"

/* filehandler */
#include "lib/filehandler/filehandler.h"
#include "lib/logger/logger.h"

/* WIN - HTTP server */
#include "lib/http/http.hpp"

/* linalg */
#include "lib/matrix/matrix.hpp"
#include "lib/calculator/calculator.hpp"

/* livsforsikringsmatematik */
#include "lib/insurance/insurance.hpp"
#include "lib/mortality/mortality.hpp"

/* London */
#include "lib/london/london.hpp"

#include "lib/block/block.hpp"

#include "lib/basecamp/basecamp.hpp"

#include "lib/walker/walker.hpp"

#include "lib/png/png.h"

void HTTPServer()
    {
    HTTP http; http.start(80, "127.0.0.1", "data/public_html");
    };

void london(int decksize)
    {
    London::Game(0, decksize);    
    };

int main(int argc, char *argv[])
    {
    HTTPServer();
    return 0;
    };