# Block
Block contains model & controller for easy controll over graphical applications. So far the graphical is centered around windows [winapi](https://stevedonovan.github.io/winapi/api.html). <br />

> Approach
```c++
// Within (int WINAPI WinMain)
/// Create LondonObjects
London::Graphics::LondonObject l;
/// Add one or more to the static lController
London::Graphics::lController.lObjects.push_back(&l);

// Within (LRESULT CALLBACK WndProc)
// in case WM_PAINT:
/// Add repaint & turn method
London::Graphics::repaint(hwnd, London::Graphics::lController.lObjects);
London::Graphics::turn(London::Graphics::lController.lObjects);
```

> lController
```
std::vector<std::vector<int> > map;  // which is used by all LondonObjects
Walker *w;                           // for randomWalk within map
Astar astar;                         // for shortest path from (x,y) to (dx,dy)
std::vector<int> notTraversable;     // For none traversable values within map
```

> LondonObject
```
                                     // Access to all elements in lController
getMapValue(int x, int y)
setMapValue(int x, int y, int v)
getCols()
getRows()

int x,y,dx,dy;                       // Position now & destination
int step;                            // Position in path
std::vector<Node> path;              // Contains positions to traverse

virtual cpaint();                    // Is called in London::Graphics::repaint
virtual turn();                      // Is called in London::Graphics::turn

origin(int x, int y)                 // set x & y
move()                               // set x & y to position in path, step++
destination(int dx, int dy)          // set path = astar.shortest_path(x, y, dx, dy)
randomWalk()                         // set path = w->randomWalk()
```

> Example
I tryed to implement some child classes of LondonObject. The result can be seen in the folder basecamp.

