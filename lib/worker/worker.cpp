#include "worker.hpp"

Worker::Worker(int fromX, int fromY, int toX, int toY)
    {
    hFont = (HFONT) CreateFont (FONT_SIZE, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,     
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
            DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
    
    origin(fromX,fromY);
    backpack = 0;
    };

void Worker::cpaint(HDC &hdc, RECT &rcWindow)
    {
    SetTextColor(hdc, RGB(255,0,0));

    SelectObject(hdc,hFont);

    // home
    TextOut(hdc, this->ox*STEP, this->oy*STEP, (std::string("#")).c_str(), (std::string("#")).length());
    
    // worker
    if(backpack > 0 && backpack != BACKPACK)
        TextOut(hdc, this->x*STEP, this->y*STEP, (std::string("G")).c_str(), (std::string("G")).length());
    else if(backpack == BACKPACK)
        TextOut(hdc, this->x*STEP, this->y*STEP, (std::string("O")).c_str(), (std::string("O")).length());
    else if(step >= path.size()-1)
        TextOut(hdc, this->x*STEP, this->y*STEP, (std::string("o")).c_str(), (std::string("o")).length());
    else
        TextOut(hdc, this->x*STEP, this->y*STEP, (std::string("o")).c_str(), (std::string("o")).length());

    // steps
    for(int i = 0; i < step; i++)
        TextOut(hdc, path[i].x*STEP, path[i].y*STEP, (std::string(".")).c_str(), (std::string(".")).length());

    SetTextColor(hdc, RGB(0,0,0)); 
    };

void Worker::turn()
    {
    // Move if not working
    if(!work())
        move();
    };

bool Worker::work()
    {
    setNeighbourhood(VIEW_ZONE);   

    return resourceFieldSearchExtractReturn();
    };

bool Worker::resourceFieldSearchExtractReturn()
    {
    if(dx == ox && dy == ox && x != ox && y != oy) return false; // on its way home
        
    if(dx != ox && dy != oy && backpack == BACKPACK)
        {
        destination(ox, oy, TRUE); // Go home  
        return false; 
        }
    
    if(getMapValue(x,y) == RESOURCE && backpack < BACKPACK) // Grind resource if any
        {
        if(grindResource(x,y, backpack == BACKPACK-1)) backpack++;
        return true;
        }
    
    if(getMapValue(dx, dy) == RESOURCE) return false; // on its way to resourcefield
    
    if(x == ox && y == oy) 
        {
        backpack = 0; // at home
        return false;
        }
    
    for(int i = 0; i < neighbours.size(); i++)
        {
        if(getMapValue(neighbours[i].x,neighbours[i].y) == RESOURCE && backpack < BACKPACK) 
                {
                destination(neighbours[i].x, neighbours[i].y, TRUE); // Goto resourcefield
                return false;
                }
        }
    return false;
    };
