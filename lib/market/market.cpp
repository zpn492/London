#include "market.hpp"

Market::Market(int fromX, int fromY, int toX, int toY, const char *goods)
    {
    hFont = (HFONT) CreateFont (FONT_SIZE, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,     
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
            DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
    
    origin(fromX,fromY);
    ((this->filenamePrice = "data\\public_html\\js\\") += goods) += "Price.js";
    ((this->filenameProduct = "data\\public_html\\js\\") += goods) += "Product.js";
    (this->labelPrice = goods) += "price";
    (this->labelProduct = goods) += "product";
    };

void Market::cpaint(HDC &hdc, RECT &rcWindow)
    {
    SetTextColor(hdc, RGB(255,0,0));

    SelectObject(hdc,hFont);

    SetTextColor(hdc, RGB(0,0,0)); 
    };

void Market::turn()
    {
    randomWalk();
    for(int i = 0; i < path.size()-1; i++)
        {       
        origin(path[step].x, path[step].y);
        datasetPrice.push_back(path[step].x);
        datasetProduct.push_back(path[step].y);
        labels.push_back(filehandler::int_to_string(turner));
        turner++;
        }
    
    filehandler::linechart(this->filenamePrice.c_str(), labels, datasetPrice, this->labelPrice);
    filehandler::linechart(this->filenameProduct.c_str(), labels, datasetProduct, this->labelProduct);
    };