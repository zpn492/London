# Basecamp
Basecamp is a child class of LondonObject which can be found in the folder block. Basecamp takes a position (x, y) & a number of starting cubes. Cube is child class of LondonObject.

> Approach
```
Each cube will:
* Search for a resource until it is within the view zone
* Go to the resource position
* Grind 10 times
* Go back to basecamp & empty the resources
* Start over

Basecamp will upon resource achieving, buy a new cube.
```

> Example
```c++
// Create a basecamp object with four cubes.
// Position the basecamp at (32, 24)
Basecamp b1(32, 24, 4);

//Add basecamp to controller (painting & logic)
London::Graphics::lController.lObjects.push_back(&b1);
```

# Links
> windows.h
> libgdi32.a