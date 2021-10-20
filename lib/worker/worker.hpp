#ifndef __WORKER__
#define __WORKER__

#include "../../SimpleJust.h"

#include "../../lib/filehandler/filehandler.h"
#include "../../lib/block/block.hpp"

using namespace London::Graphics;

class Worker : public LondonObject
    {
public:
    Worker(int fromX, int fromY, int toX, int toY);

    void cpaint(HDC &hdc, RECT &rcWindow) override;

    void turn() override;

private:
    /**
     * Search for resourcefield
     * If found, goto field
     * Grind until backback is full
     * Return home, to unload
     * ... Start over.
     */
    bool resourceFieldSearchExtractReturn();

    bool work();

    HFONT hFont;

    int backpack;
    };



#endif