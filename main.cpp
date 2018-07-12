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

void HTTPServer()
    {
    HTTP http; http.start(80, "127.0.0.1", "data/public_html");
    };

void london(int decksize)
    {
    London::Game(0, decksize);    
    };
    
using namespace linalg;

int main(int argc, char *args[])
    {   
    london(atoi(args[1]));
    return 0;
    };