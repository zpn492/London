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
#include "src/london/london.hpp"

#include "src/block/block.hpp"

#include "src/basecamp/basecamp.hpp"

#include "src/walker/walker.hpp"

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