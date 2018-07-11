# AStar
Peter-Benja collab project

AStar is using the power of the Bellman-Ford algorithm, to find the shortest path from A to B,
where A and B is positions in a two dimensional space.

Example: <br />
#include "Astar.hpp" <br />
Astar astar; <br />
int fromX=0, fromY=0, toX=2, toY=2; <br />
std::vector<std::vector<int> > map = astar.generateMap(3,3); <br />
std::vector<Node> path = astar.shortest_path(fromX, fromY, toX, toY,  map); <br />

path[0] contains start position = (fromX, fromY); <br />
path[n] contains end position = (toX, toY); <br />
In between 0 .. n, you'll find which nodes you should step through, to get from A to B fastest. 