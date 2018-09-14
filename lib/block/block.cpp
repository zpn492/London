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
    {
    return match(p.x, p.y);
    };

bool London::Pos::match(int x, int y) 
    { return this->x == x && this->y == y ? 1 : 0; };


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

std::vector<London::Pos> London::Graphics::LondonObject::getNeighbourhood(int view)
    {
    int c2 = view/2;
    int r2 = view/2;
    int tx, ty;
    std::vector<London::Pos> neighbours;
    for(int i = -c2; i < c2; i++)
        {
        for(int j = -r2; j < r2; j++)
            {
            tx = i + x;
            ty = j + y;
            if(tx < 0 || tx >= getCols() || ty < 0 || ty >= getRows())
                continue;
            neighbours.push_back(London::Pos(tx, ty));
            }
        }
    return neighbours;
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
    x = path[step].x;
    y = path[step].y;
    step++;
    };

void London::Graphics::LondonObject::origin(int x, int y)
    {
    this->x = x;
    this->y = y;
    };

void London::Graphics::LondonObject::destination(int dx, int dy, bool calculatePath)
    {
    step = 0;
    this->dx = dx;
    this->dy = dy;
    if(calculatePath)
        path = lController.astar.shortest_path(x, y, dx, dy, (*map), 
            London::Graphics::lController.notTraversable);
    else
        path.push_back(Node(0, x, y));
    };

std::vector<Node> London::Graphics::LondonObject::randomWalk()
    {
    std::vector<Node> path2;
    int tx = 0; int ty = 0;
    path.clear();
    // Search for a path which is keep the object within the map.
    // 
    while(path.empty())
        {
        path2 = London::Graphics::lController.w->randomWalk
            (London::Graphics::lController.seed);
        
        for(int i = 0; i < path2.size(); i++)
            {
            tx = path2[i].x + x;
            ty = path2[i].y + y;
            if(tx < 0 || tx >= map->size() || 
                ty < 0 || ty >= map->at(0).size())
                {
                break; // path would have led the object out of bounds
                }
            bool b = false;
            for(int j = 0; j < London::Graphics::lController.notTraversable.size(); j++)
                if(London::Graphics::lController.notTraversable[j] == map->at(tx)[ty])
                    b = true; // position is not traversable
            if(b)
                break; // position is not traversable
            if(path.empty())
                path.push_back(Node(0, x, y));
            path.push_back(Node(0, tx, ty));
            }
        }
    // reset steps & destination
    step = 0; dx = tx; dy = ty;
    return path;
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

    for(int i = 0; i < llObjects.size(); i++) llObjects[i]->cpaint(hdc, rcWindow);

    // Draw MAP
    HFONT hFont= (HFONT) CreateFont (12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
    SelectObject(hdc,hFont);
        
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