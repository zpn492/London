# Market <br />
> Simulerer en udvikling i pris og en produktion ud fra block/block.hpp::randomWalk();

> Skriver til data\\public_html\\js\\[goods]Price.js

> Skriver til data\\public_html\\js\\[goods]Product.js

> Approach
```c++
// Within (int WINAPI WinMain)
/// Create LondonObjects
Market iron(32,16,32,16, "iron");
Market wool(32,16,32,16, "wool");

/// Add one or more to the static lController
London::Graphics::lController.lObjects.push_back(&iron);
London::Graphics::lController.lObjects.push_back(&wool);

// Within (LRESULT CALLBACK WndProc)
// in case WM_PAINT:
/// Add repaint & turn method
London::Graphics::repaint(hwnd, London::Graphics::lController.lObjects);
London::Graphics::turn(London::Graphics::lController.lObjects);
```

## Afhængighed
* block
* filehandler
* walker