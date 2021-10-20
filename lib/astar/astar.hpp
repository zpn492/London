#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>

#ifndef __ASTAR__
#define __ASTAR__

class Node
    {
public:
    int x;
    int y;
    bool traversable;
    Node(int EDGE, int X, int Y);
    Node() {};
    ~Node() {};
    std::vector<Node*> neighbours;
    int estimatedCost;
    int edge;
    int connectionX;
    int connectionY;
    };

/** 
 * 30-03-2018
 * ozuware/PlaceInSpace
 * 
 * Astar, is used to find the shortest path from A to B
 * where A and B, are positions within a 2-dimensional vector.
 *
 * Example:
 * #include "Astar.hpp"
 * Astar astar;
 * int fromX=0, fromY=0, toX=2, toY=2;
 * std::vector<std::vector<int> > map = astar.generateMap(3,3);
 * std::vector<Node> path = astar.shortest_path(fromX, fromY, toX, toY,  map);
 *
 * path[0] contains start position = (fromX, fromY)
 * path[n] contains end position = (toX, toY);
 * In between 0 .. n, you'll find which nodes you should step through, to get from A to B fastest. 
 *
 * shortest path is found using power of the,
 * Bellman-Ford algorithm: https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
 */
class Astar
    {
    std::vector<Node> nodes;

    std::vector<int> notTraversable;

    /* Find all neighbours to a node. 
     * foreach match update n.neighbours, with a reference
     */
    void find_neighbours(const std::vector<std::vector<int> > &map, Node &n);

    /* Initialize all nodes within the map
     * find neighbours foreach node,
     * Where a neighbour is a node within the same square
     * fx N = neighbour, n = node, then
     * N N N
     * N n N
     * N N N
     */
    void init(const std::vector<std::vector<int> > &map);

    /* This method is used to calculate the cost,
     * of moving to Node n.
     */
    int weight(Node n)
        {
        return n.edge;
        };
    
    /* This method will return the index of Node(x,y)
     * If no Node was found, the method will return -1 */
    int find_node_id(int x, int y);


protected:
    /*
     * Bellman-Ford(source, vertices/edge):
     * Initialize all elements to infinity = init(map)
     * Set source to zero
     * relax edges until no changes
     * check for negative-weigth cycles /* not implemented
     *
     * @map - a 2-dimensional vector[cols][rows]
     */
    std::vector<Node> find_path(int x, int y, const std::vector<std::vector<int> > &map);

public:
    /*
     * Main method in Astar, returns a list of nodes, which contains initial node, end node, 
     * and alle nodes in between which results in the fastest path between initial and end.
     *
     * @map - a 2-dimensional vector[cols][rows]
     */
    std::vector<Node> shortest_path(int fromX, int fromY, int toX, int toY, 
        const std::vector<std::vector<int> > &map, 
        const std::vector<int> &notTraversable = std::vector<int>(),
        bool debug=false);

    /* Generate 2-dimensional vector it random values between 1 and 6 
     *
     * @return vector[cols][rows]
     */
    std::vector<std::vector<int> > generateMap(int cols, int rows, int values=6);

    std::vector<Node> getNeighbourhood(int x, int y, int maxX, int maxY, int viewzone);
    };
#endif