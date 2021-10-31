#include "astar.hpp"

/* ********************************* */
/* Node */
/* ********************************* */

Node::Node(int EDGE, int X, int Y) 
    { 
    edge = EDGE; 
    x = X; 
    y = Y; 
    estimatedCost = 1000;
    connectionX = -1; 
    connectionY = -1;
    traversable = true; 
    };

/* ********************************* */
/* Astar */
/* ********************************* */

void Astar::find_neighbours(const std::vector<std::vector<int> > &map, Node &n)
    {
    for(int i = -1; i < 2; i++)
        {
        for(int j = -1; j < 2; j++)
            {
            if(find_node_id(n.x+i, n.y+j) == -1)
                continue;
            
            Node n1 = nodes[find_node_id(n.x+i, n.y+j)];
            
            if(!notTraversable.empty())
                {
                int counter = 0;
                for(int k = 0; k < notTraversable.size(); k++)
                    if(notTraversable[k] == n1.edge)
                        counter++;
                if(counter > 0)
                    continue;
                }
            
            n.neighbours.push_back(&nodes[find_node_id(n.x+i, n.y+j)]);
            }
        }
    };

void Astar::init(const std::vector<std::vector<int> > &map)
    {
    nodes.clear();
    /* Initialize nodes */
    for(int i = 0; i < map.size(); i++)
        {
        for(int j = 0; j < map[i].size(); j++)
            {
            int v = map[i][j];
            for(int k = 0; k < notTraversable.size(); k++)
                if(notTraversable[k] == v)
                    v = 1000;
            Node n(v, i, j);
            nodes.push_back(n);
            }
        }
    
    /* Find neighbours */
    for(int k = 0; k < nodes.size(); k++)
        {
        find_neighbours(map, nodes[k]);
        }        
    };

int Astar::find_node_id(int x, int y)
    {
    for(int i = 0; i < nodes.size(); i++)
        if(nodes[i].x == x && nodes[i].y == y)
            return i;
    return -1;
    };

std::vector<Node> Astar::find_path(int x, int y, const std::vector<std::vector<int> > &map)
    {
    init(map);
    nodes[find_node_id(x, y)].estimatedCost = 0;
    bool changesWereMade = true;
    while(changesWereMade)
        {
        changesWereMade = false;
        for(int i = 0; i < nodes.size(); i++)
            {
            for(int j = 0; j < nodes[i].neighbours.size(); j++)
                {
                if(nodes[i].estimatedCost > weight(nodes[i]) + 
                    nodes[i].neighbours[j]->estimatedCost )
                    {
                    nodes[i].estimatedCost = weight(nodes[i]) + 
                        nodes[i].neighbours[j]->estimatedCost;
                    nodes[i].connectionX = nodes[i].neighbours[j]->x;
                    nodes[i].connectionY = nodes[i].neighbours[j]->y;
                    changesWereMade = true;
                    }
                }
            }
        }
    return nodes;
    };

std::vector<Node> Astar::shortest_path(int fromX, int fromY, int toX, int toY, 
    const std::vector<std::vector<int> > &map,
    const std::vector<int> &notTraversable,
    bool debug)
    {
    this->notTraversable = notTraversable;
    std::vector<Node> path = find_path(toX,toY,map);
    std::vector<Node> finalPath;

    Node n = nodes[find_node_id(fromX, fromY)];

     /* add initial node */
    finalPath.push_back(n);

    while(n.connectionX != -1)
        {
        n = nodes[find_node_id(n.connectionX, n.connectionY)];
        
        /* add node to the final path */
        finalPath.push_back(n);
        
        if(n.connectionX == toX && n.connectionY == toY) break;
        }

    if(debug)
        {
        std::cout << "From: (" << fromX << ", " << fromY << "), To: (" << toX << ", " << toY << ")" << std::endl; 
        

        for(int i = 0; i < finalPath.size(); i++)
            {
            std::cout << "(" << finalPath[i].x << ", " << finalPath[i].y << ") -> ";
            //std::cout << "Initial cost: " << path[i].edge << ", Connection cost: ";
            //std::cout << path[i].estimatedCost << ", Neigbours: ";
            //for(int j = 0; j < path[i].neighbours.size(); j++)
            //    std::cout << path[i].neighbours[j]->edge << ", ";
            std::cout << "" << std::endl;
            }
        }
    return finalPath;
    };

std::vector<std::vector<int> > Astar::generateMap(int cols, int rows, int values)
    {
    std::vector<std::vector<int> > map;
    std::srand(std::time(NULL));
    int random_v;

    for(int i = 0; i < cols; i++)
        {
        std::vector<int> row;
        map.push_back(row);
        for(int j = 0; j < rows; j++)
            {
            random_v = 1+ std::rand() / ((RAND_MAX + 1u) / values);
            map[i].push_back(random_v);
            }
        }
    return map;
    };

std::vector<Node> Astar::getNeighbourhood(int x, int y, int maxX, int maxY, int viewzone)
    {
    int c2 = viewzone/2;
    int r2 = viewzone/2;
    int tx, ty;
    std::vector<Node> neighbours;
    for(int i = -c2; i < c2; i++)
        {
        tx = i + x;
        if(tx < 0 || tx >= maxX)
            continue;
        for(int j = -r2; j < r2; j++)
            {
            ty = j + y;
            if(tx < 0 || tx >= maxX || ty < 0 || ty >= maxY)
                continue;
            else
                neighbours.push_back(Node(0, tx, ty));
            }
        }
    return neighbours;
    };