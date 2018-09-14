#include "basecamp.hpp"
Gauss::Gauss(std::vector<std::vector<double> > &g, int times, int x, int y)
    {
    this->g = g;
    this->times = times;
    this->direction = direction;
    origin(x, y);
    };

void Gauss::cpaint(HDC &hdc, RECT &rcWindow)
    {
    for(int i = 0; i < g.size(); i++)
        {
        for(int j = 0; j < g[i].size(); j++)
            {
            int c = std::min(255, (int)(g[i][j]));
            for(int k = 0; k < times; k++)
                {
                for(int v = 0; v < times; v++)
                    {
                    SetPixel(hdc, x+(i*times)+k, y+(j*times)+v, RGB(c,c,c));
                    }
                }
            }
        }
    };

void Gauss::turn() {};

Cube::Cube(int fromX, int fromY, int toX, int toY)
    { 
    alive = TRUE; atWork = FALSE;
    origin(fromX, fromY);
    originX = fromX; originY = fromY;
    ll = fromX*STEP;  rr = fromY*STEP; 
    resource = 0;
    fund_resource = FALSE;
    going_home = FALSE;
    destination(toX, toY, FALSE);
    ID = London::Graphics::GLOBAL_ID++;
    };

void Cube::cpaint(HDC &hdc, RECT &rcWindow)
    {
    RECT r = rcWindow;
    
    r.left = ll;
    r.top = ll;
    r.right = rr;
    r.bottom = rr;
    SetTextColor(hdc, RGB(255,0,0));
    HFONT hFont= (HFONT) CreateFont (FONT_SIZE, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,     
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
            DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));

    SelectObject(hdc,hFont);

    if(fund_resource && !going_home && x != dx && y != dy)
        TextOut(hdc, dx*STEP, dy*STEP, (std::string("@")).c_str(), (std::string("@")).length());

    if(fund_resource && !going_home && x != dx && y != dy)
        {
        TextOut(hdc, path[0].x*STEP, path[0].y*STEP, m.c_str(), m.length());
        for(int i = 0; i < step; i++)
            TextOut(hdc, path[i].x*STEP, path[i].y*STEP, d.c_str(), d.length());
        }
    else if(fund_resource && going_home)
        {
        TextOut(hdc, path[0].x*STEP, path[0].y*STEP, g.c_str(), g.length());
        for(int i = 0; i < step; i++)
            TextOut(hdc, path[i].x*STEP, path[i].y*STEP, d.c_str(), d.length());
        }
    else if(fund_resource && !going_home && x == dx && y == dy)
        TextOut(hdc, x*STEP, y*STEP, w.c_str(), w.length());
    else
        {
        TextOut(hdc, path[0].x*STEP, path[0].y*STEP, s.c_str(), s.length());
        if(step >= path.size()-1)
            TextOut(hdc, path[path.size()-1].x*STEP, path[path.size()-1].y*STEP, e.c_str(), e.length());
        for(int i = 0; i < step; i++)
            TextOut(hdc, path[i].x*STEP, path[i].y*STEP, d.c_str(), d.length());
        }

    SetTextColor(hdc, RGB(0,0,0)); 
    };

void Cube::turn() 
    {
    if(step < path.size())
        {
        ll = path[step].x*STEP;
        rr = path[step].y*STEP;
        move();
        }
    else 
        {
        // Destination was reached.
        // Give this object a new rute.
        // Or grind if standing on a resource
        atWork = FALSE;
        }
    };

void Cube::work()
    {    
    if(x == originX && y == originY && going_home)
        {
        atWork = FALSE;
        fund_resource = FALSE;
        going_home = FALSE;
        resource = 0;
        pretty_print(std::string("Returned home"));
        return;
        }
    
    // If your staying on a resource, grind
    else if(((fund_resource && x == dx && y == dy) || resource == BACKPACK) && !going_home)
        {
        // If your filled your back with resources, go home to basecamp
        if(resource == BACKPACK)
            {
            setMapValue(x, y, 2);
            destination(originX, originY);
            going_home = TRUE;
            pretty_print(std::string("Going home"));
            }
        else
            {
            resource += 1;
            pretty_print(std::string("Grinding"));
            }
        return;
        }
    // Search for a resource.
    // If you can see a resource field & your not on your way home, go to that field
    else
        {        
        // Look around
        if(!fund_resource)
            {
            std::vector<London::Pos> neighbours = getNeighbourhood((int)VIEW_ZONE);
            for(int i = 0; i < neighbours.size(); i++)
                {
                if(LockOrUnlcokOccupationIfValue(neighbours[i].x, 
                    neighbours[i].y, RESOURCE))
                    {
                    destination(neighbours[i].x, neighbours[i].y);
                    fund_resource = TRUE;
                    pretty_print(std::string("Found resource"));
                    return;
                    }
                }
            }
        // Search in another area
        if(!fund_resource)
            {
            // Fill path[] with nodes
            // Search for path, where the end position is not occupied
            randomWalk();

            pretty_print(std::string("Search for resource"));
            }
        }
    };

Basecamp::Basecamp(int x, int y, int t_villages)
    {
    origin(x, y);
    destination(x, y);
    resources = 0;
    for(int i = 0; i < t_villages; i++)
        {
        Cube v(x, y, x, y); 
        villages.push_back(v);
        }    
    };

void Basecamp::beginWork(int ID)
    {
    villages[ID].work();
    };

std::thread Basecamp::spawnThread(int ID)
    {
    return std::thread(Basecamp::beginWork, this, ID);
    };

void Basecamp::cpaint(HDC &hdc, RECT &rcWindow)
    {
    // Draw basecamp
    HFONT hFont= (HFONT) CreateFont (FONT_SIZE, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
            DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
    SelectObject(hdc,hFont);
    SetTextColor(hdc, RGB(255,0,0));
    
    //Draw villager count
    std::string ss = filehandler::int_to_string((int)villages.size()); ss = "Villagers: " + ss;
    TextOut(hdc, 1*STEP, 1*STEP, ss.c_str(), ss.length());

    //Draw resource count
    ss = filehandler::int_to_string((int)resources); ss = "Resources: " + ss;
    TextOut(hdc, 1*STEP, 3*STEP, ss.c_str(), ss.length());

    int counter = 0;
    for(int i = 0; i < getCols(); i++)//for(int i = 0; i < map->size(); i++)
    for(int j = 0; j < getRows(); j++)//for(int j = 0; j < map->at(i).size(); j++)
    if(getMapValue(i, j) == 7)
        counter += 1;    
    ss = filehandler::int_to_string(counter); ss = "Resourcefields left: " + ss;
    TextOut(hdc, 1*STEP, 5*STEP, ss.c_str(), ss.length());
    
    TextOut(hdc, x*STEP, y*STEP, (std::string("#")).c_str(), (std::string("#")).length());

    SetTextColor(hdc, RGB(0,0,0));
    for(int i = 0; i < getCols(); i++)
    for(int j = 0; j < getRows(); j++)
    if(getMapValue(i, j) == 3)
        TextOut(hdc, i*STEP, j*STEP, (std::string("o")).c_str(), (std::string("o")).length());
    
    // Draw villages
    for(int i = 0; i < villages.size(); i++) villages[i].cpaint(hdc, rcWindow);
    };

void Basecamp::turn()
    {
    //Deploy villages    
    std::vector<std::thread> threads;
    for(int i = 0; i < villages.size(); i++)
        {           
        //Take turn
        villages[i].turn();
        if(!villages[i].atWork && villages[i].alive)
            {
            if(x == villages[i].originX && y == villages[i].originY && villages[i].going_home)
                {
                resources += villages[i].resource;
                //Buy a new villager
                if(resources >= WORKER_COST)
                    {
                    Cube v(x, y, x, y); 
                    villages.push_back(v);
                    }
                }
            villages[i].atWork = TRUE;
            threads.push_back(spawnThread(i));
            }
        }
        for(int i = 0; i < threads.size(); i++)
            threads[i].join();
        
    };