#ifndef __BASECAMP__
#define __BASECAMP__

#include "../../SimpleJust.h"

#include "../london/london.hpp"
#include "../block/block.hpp"

class Gauss : public London::Graphics::LondonObject
    {
public:
    Gauss(std::vector<std::vector<double> > &g, int times=10, int x=1, int y=1);

    void cpaint(HDC &hdc, RECT &rcWindow) override;

    void turn() override;

    std::vector<std::vector<double> > g;
    int times;
    int direction;
    };

class Cube : public London::Graphics::LondonObject
    {
public:    
    Cube(int fromX, int fromY, int toX, int toY);

    void cpaint(HDC &hdc, RECT &rcWindow) override;

    void turn() override;

    void work();

    std::string s = "S"; // Searching
    std::string e = "E"; // End
    std::string d = ".";
    std::string w = "W"; // Working
    std::string g = "G"; // Going home
    std::string m = "M"; // Moving towards resource
    int resource;
    bool fund_resource, going_home;
    int originX; int originY;
    double ll, rr;
    bool alive, atWork;
    };

class Basecamp : public London::Graphics::LondonObject
    {
protected:
    std::vector<Cube> villages;
public:
    //Basecamp(int x, int y, int t_villages, std::vector<std::vector<int> > &map);
    Basecamp(int x, int y, int t_villages);

    void cpaint(HDC &hdc, RECT &rcWindow) override;

    void turn() override;

    void beginWork(int ID);

    std::thread spawnThread(int ID);

    int resources;
    };
#endif