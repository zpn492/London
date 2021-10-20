#ifndef __MARKET__
#define __MARKET

#include "../../SimpleJust.h"

#include "../../lib/filehandler/filehandler.h"
#include "../../lib/block/block.hpp"

using namespace London::Graphics;

class Market : public LondonObject
    {
public:
    Market(int fromX, int fromY, int toX, int toY, const char *goods="goods");

    void cpaint(HDC &hdc, RECT &rcWindow) override;

    void turn() override;
private:
    HFONT hFont;
    std::vector<std::string> labels;
    std::vector<double> datasetPrice;
    std::vector<double> datasetProduct;
    int turner = 0;
    std::string filenamePrice;
    std::string filenameProduct;
    std::string labelPrice;
    std::string labelProduct;
    };

#endif