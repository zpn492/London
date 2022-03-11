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

void requestAdress()
    {
    Sock web(443, "147.29.175.24", 1);			// Initialize Web object & Connect to a remote server
    const int maxbuffer = 1024;					// Define max buffer size
    char buffer1[maxbuffer];					// Intialize a buffer
    web.tsend("GET /rest/soeg/R%C3%B8nneholmsvej%202610 HTTP/1.1\n\n");	// Send a GET request
    web.trecv(buffer1, maxbuffer);				// Store response in buffer
    std::cout << buffer1 << std::endl;
    web.tcleanUp();								// Close connection & Free memory
    };

int main(int argc, char *argv[])
    {
    requestAdress();
    return 0;
    };