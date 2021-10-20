#include "block.hpp"

/* **************************************** */
/* London */
/* **************************************** */

void London::vsleep(int times)
    {
    for(int i = 0; i < times; i++)
        usleep(100000);
    };

London::Pos::Pos(int x, int y)
    {this->x = x; this->y = y;};

London::Pos::operator==(Pos &p)
    {return match(p.x, p.y);};

bool London::Pos::match(int x, int y) 
    {return this->x == x && this->y == y ? 1 : 0;};


/* **************************************** */
/* London::Graphics::LondonController */
/* **************************************** */

London::Graphics::LondonController::LondonController()
    {
    //Define walker, with a 5x5 Gauss matrix
    Walker ww(5,5); w = &ww;

    // Seed the random walk algorithm
    const char* filename = "city_160.png";
    seed = w->seed(filename);

    // Generate map
    map = astar.generateMap(64, 48); 

    // Set some resource fields
    map.at(20)[20] = 7; map.at(5)[10] = 7; map.at(10)[5] = 7; map.at(15)[5] = 7; 
    map.at(10)[15] = 7; map.at(10)[10] = 7; map.at(30)[20] = 7;
    map.at(30)[30] = 7; map.at(60)[40] = 7; map.at(25)[45] = 7;
    
    // Set values which is not traversable
    notTraversable.push_back(3);

    // Find neighbourghood for each position
    for(int x =  0; x < map.size(); x++)
        {
        for(int y = 0; y < map.at(x).size(); y++)
            {
            Pos p(x,y);
            std::vector<Node> t = astar.getNeighbourhood(x,y,map.size(),map.at(x).size(),VIEW_ZONE);
            neighbours.insert ( std::pair<Pos, std::vector<Node> > (p, t) );    
            }
        }          
    };

/* **************************************** */
/* London::Graphics::LondonObject */
/* **************************************** */

void London::Graphics::LondonObject::pretty_print(std::string message)
    {
    std::lock_guard<std::mutex> guard(lController.lmutex);
    std::cout << "Turn: " << London::Graphics::turner << " ID: " << ID << " " << message.c_str() << ": (x, y, dx, dy) ";
    std::cout << x << ", " << y << ", " << dx << ", " << dy << std::endl; 
    };

int London::Graphics::LondonObject::getMapValue(int x, int y)
    {
    //std::lock_guard<std::mutex> guard(lController.lmutex);
    if(x < 0 || x >= map->size() || y < 0)
        return -1;
    else if(y >= map->at(x).size())
        return -1;
    else
        return map->at(x)[y];
    };

bool London::Graphics::LondonObject::setMapValue(int x, int y, int value)
    {
    if(x < 0 || x >= map->size() || y < 0)
        return false;
    else if(y >= map->at(x).size())
        return false;
    else
        {
        map->at(x)[y] = value;
        return true;
        }
    };

bool London::Graphics::LondonObject::LockOrUnlcokOccupation(int x, int y)
    {
    if(!isOccupied(x, y))
        {
        occupy(x, y);
        return true;
        }
    else if(x == dx && y == dy)
        {
        releaseOccupation(x, y);
        return true;
        }
    else
        return false;
    };

bool London::Graphics::LondonObject::LockOrUnlcokOccupationIfValue(int x, int y, int v)
    {
    std::lock_guard<std::mutex> guard(lController.lmutex);
    int field = getMapValue(x, y);
    if(field == v)
        return LockOrUnlcokOccupation(x, y);
    else
        return false;
    };

bool London::Graphics::LondonObject::isOccupied(int x, int y)
    {
    Pos p(x, y);
    std::map<Pos, LondonObject>::iterator occupiedIt = lController.occupied.find(p);
    if (occupiedIt != lController.occupied.end())
        return true;
    else
        return false;
    };

bool London::Graphics::LondonObject::occupy(int x, int y)
    {
    if(!isOccupied(x, y))
        {
        Pos p(x, y);
        London::Graphics::LondonObject l;
        lController.occupied.insert ( std::pair<Pos, London::Graphics::LondonObject> (p, l) );
        return true;
        }
    else
        return false;
    };

bool London::Graphics::LondonObject::releaseOccupation(int x, int y)
    {
    if(isOccupied(x, y))
        {
        Pos p(x, y);
        std::map<Pos, London::Graphics::LondonObject>::iterator occupiedIt = 
            lController.occupied.find(p);
        delete lController.occupied.find(p)->second.map;
        lController.occupied.erase(occupiedIt);
        return true;
        }
    else
        {
        return false;
        }
    };

London::Graphics::LondonObject London::Graphics::LondonObject::GetObjectAtOccupation(int x, int y)
    {
    Pos p(x, y);
    if(isOccupied(x, y))
        return lController.occupied.find(p)->second;
    else
        {
        std::cout << "LondonObject was not found, exiting program" << std::endl;
        assert("Out of bounds");
        }
    London::Graphics::LondonObject l;
    return l;
    };

int London::Graphics::LondonObject::getCols()
    {
    if(lController.map.empty())
        return -1;
    else
        return lController.map.size();;
    };

int London::Graphics::LondonObject::getRows()
    {
    //std::lock_guard<std::mutex> guard(lmutex);
    if(lController.map.empty())
        return -1;
    else if(lController.map[0].empty())
        return -1;
    else
        return lController.map[0].size();
    };

void London::Graphics::LondonObject::move()
    { 
    if(!path.empty()) 
        {
        if(step < path.size()) {
        this->x = path[step].x;
        this->y = path[step].y;
        //std::cout << "step: " << step;
        //std::cout << " x: " << path[step].x;
        //std::cout << " y: " << path[step].y << std::endl;
        step++;
        return;
        }

        }
    randomWalk();
    };

void London::Graphics::LondonObject::origin(int x, int y)
    {
    this->x = x;
    this->y = y;
    this->ox = x;
    this->oy = y;
    };

void London::Graphics::LondonObject::destination(int dx, int dy, bool calculatePath)
    {
    step = 0;
    this->dx = dx;
    this->dy = dy;
    if(calculatePath)
        {
        this->path.clear();
        this->path = lController.astar.shortest_path(x, y, dx, dy, (*map), 
            London::Graphics::lController.notTraversable);
        }
    else
        this->path.push_back(Node(0, dx, dy));
    };

bool London::Graphics::LondonObject::outOfMap(int tx, int ty)
    {
    return tx < 0 || tx >= map->size() || 
                ty < 0 || ty >= map->at(0).size();
    };

bool London::Graphics::LondonObject::notTraversable(int tx, int ty)
    {
    for(int j = 0; j < London::Graphics::lController.notTraversable.size(); j++)
    {
    if(London::Graphics::lController.notTraversable[j] == map->at(tx)[ty])
        return true;
    } 
    return false;
    };

void London::Graphics::LondonObject::randomWalk()
    {
    std::vector<Node> path2; this->path.clear();
    // Search for a path which will keep the object within the map.
    // 
    while(this->path.empty())
        {
        int tx = 0; int ty = 0; bool b = false;
        dx = x; dy = y;

        path2 = London::Graphics::lController.w->randomWalk
            (London::Graphics::lController.seed);
        
        for(int i = 0; i < path2.size(); i++) 
            {
            tx = path2[i].x + x;
            ty = path2[i].y + y;
            if(outOfMap(tx, ty)) break;  // path would have led the object out of bounds
            if(notTraversable(tx, ty)) break; // path is not traverseble
            destination(tx, ty, FALSE); // add step to path
            }
        }
    // reset steps & destination
    path2.clear();
    step = 0; 
    };

bool London::Graphics::LondonObject::grindResource(int x, int y, bool erase=false)
        {
        if(outOfMap(x,y)) return false;
        
        if(erase) map->at(x)[y] = 2;

        return (map->at(x)[y] == RESOURCE || erase);
        };

void London::Graphics::LondonObject::randomWalkBernoulli()
    {
    int tx = 0; int ty = 0;
    srand(time(NULL));
    int d = rand() % VIEW_ZONE + 1; // 1-5
    int upOrDown = rand() % 2 == 0 ? -1 : 1; // 0-1
    
    tx = std::max(0, std::min(x+d*upOrDown, (int)map->size()-1));
    ty = std::max(0, std::min(y+d*upOrDown, ((int)map->at(tx).size()-1)));
    destination(tx, ty);
    };

void London::Graphics::LondonObject::setNeighbourhood(int view)
    {
    Pos p(x,y);
    std::map<Pos, std::vector<Node>>::iterator occupiedIt = lController.neighbours.find(p);
    if (occupiedIt != lController.neighbours.end())
        this->neighbours = lController.neighbours.find(p)->second;
    };

/* **************************************** */
/* London::Graphics */
/* **************************************** */

void London::Graphics::repaint(HWND hwnd, std::vector<LondonObject*> &llObjects)
    {
    PAINTSTRUCT ps;
    RECT rcWindow;
    HDC hdc;
    HBITMAP hbmMem, hbmOld;
    HBRUSH hbrBkGnd;
    HFONT hfntOld;
    hdc = BeginPaint(hwnd, &ps);

    //
    // Get the size of the client rectangle.
    //
    GetClientRect(hwnd, &rcWindow);
    
    //
    // Create a compatible DC.
    //
    hdc = CreateCompatibleDC(ps.hdc);

    //
    // Create a bitmap big enough for our client rectangle.
    //
    hbmMem = CreateCompatibleBitmap(ps.hdc,
            rcWindow.right-rcWindow.left,
            rcWindow.bottom-rcWindow.top);

    //
    // Select the bitmap into the off-screen DC.
    //
    hbmOld = (HBITMAP)SelectObject(hdc, hbmMem);

    //
    // Erase the background.
    //    
    hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    FillRect(hdc, &rcWindow, hbrBkGnd);
    DeleteObject(hbrBkGnd);
    
    //
    // Render the image into the offscreen DC.
    //
    SetBkMode(hdc, TRANSPARENT);

     // Draw MAP
    HFONT hFont= (HFONT) CreateFont (FONT_SIZE, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
    SelectObject(hdc,hFont);

    // Draw Gameboard
    for(int x =  0; x < London::Graphics::lController.map.size(); x++)
        {
        for(int y = 0; y < London::Graphics::lController.map.at(x).size(); y++)
            {
            for(int j = 0; j < London::Graphics::lController.notTraversable.size(); j++)
                if(London::Graphics::lController.notTraversable[j] == London::Graphics::lController.map.at(x)[y])
                    TextOut(hdc, x*STEP, y*STEP, (std::string("o")).c_str(), (std::string("o")).length());
            }
        }

    for(int i = 0; i < llObjects.size(); i++) llObjects[i]->cpaint(hdc, rcWindow);
    
    //
    // Blt the changes to the screen DC.
    //
    BitBlt(ps.hdc,
           rcWindow.left, rcWindow.top,
           rcWindow.right-rcWindow.left, rcWindow.bottom-rcWindow.top,
           hdc,
           0, 0,
           SRCCOPY);
    
    //
    // Done with off-screen bitmap and DC.
    //
    SelectObject(hdc, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdc);
    
    EndPaint(hwnd, &ps);

    // Repaint window
    InvalidateRect(hwnd, 0, TRUE);
    };

void London::Graphics::turn(std::vector<LondonObject*> &llObjects)
    {
    for(int i = 0; i < llObjects.size(); i++) llObjects[i]->turn();
    turner++;
    };