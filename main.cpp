/* Handles all standard includes */
#include "SimpleJust.h"

/* filehandler */
#include "lib/filehandler/filehandler.h"

#ifdef __WIN32
#include "lib/logger/logger.h"
#endif

/* WIN - HTTP server */
#ifdef __WIN32
#include "lib/http/http.hpp"
#endif

/* linalg */
#include "lib/matrix/matrix.hpp"
#include "lib/calculator/calculator.hpp"

/* livsforsikringsmatematik */
#include "lib/insurance/insurance.hpp"
#include "lib/mortality/mortality.hpp"

/* London */
#ifdef __WIN32
#include "lib/london/london.hpp"

#include "lib/block/block.hpp"

#include "lib/basecamp/basecamp.hpp"
#endif

#include "lib/walker/walker.hpp"

#include "lib/png/png.h"

#ifdef __WIN32
void HTTPServer()
    {
    HTTP http; http.start(80, "127.0.0.1", "data/public_html");
    };


void london(int decksize)
    {
    London::Game(0, decksize);    
    };
#endif

/* Example of the walker */
std::vector<Node> randomWalk(int size, Walker w, std::vector<std::vector<double> > &data)
    {
    std::vector<Node> path = w.randomWalk(data);
    if(path.size() != size)
        path = randomWalk(size,w,data);
    else
        {
        for(int i = 0; i < path.size(); i++)
            printf("%i:%i\n", path[i].x, path[i].y);
        printf("******\n");
        }
    return path;
    };

void printRandomWalk(int size)
    {
    Walker w(5,5);
    std::vector<std::vector<double> > data = w.seed("city_160.png");
    int c;
    while(1)
        {
        c=getchar();
        switch(c)
            {
            case '.': break;
            default: randomWalk(size,w,data);
            }
        }
    };
/* End of example */

int main(int argc, char *argv[])
    {
    printf("Internet engineering task force!\n");
    
    return 0;
    };