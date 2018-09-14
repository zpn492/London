#ifndef __BLOCK__
#define __BLOCK__

#include "../../SimpleJust.h"

#include "../../lib/astar/astar.hpp"
#include "../../lib/walker/walker.hpp"

/**
 * This file contains view mobility.
 */

namespace London
    {
    /**
     * this method is build upon usleep:
     * http://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html
     *
     * System sleeps for 100000 microseconds * times
     */
    void vsleep(int times=9);
    
    class Pos
        {
    public:
        Pos(int x, int y);
        operator==(Pos &p);
        bool match(int x, int y);

        const std::string &getId() const {
            return s;
        };

        const std::string &getName() const {
            return s;
        };

        operator< (const Pos &p) {
            return x < p.x && y < p.y;
        }

        int x, y;
        std::string s = "s";
        };
    
    namespace Graphics
        {
        #define STEP 10
        #define VIEW_ZONE 10
        #define RESOURCE 7
        #define BACKPACK 10
        #define WORKER_COST 10
        #define FONT_SIZE 8

        static int GLOBAL_ID = 0;
        static long turner = 0;
        
        class LondonObject;
        
        /** 
         * There will only be one LondonController: lController.
         *
         * lController, will contain:
         *
         * A list of all outer LondonObjects
         * A list of all values which is not traversable
         *
         * Global map, which is shared by all LondonObjects
         * Walker which will enable randomWalk through out the map
         * Astar which will enable shortest path from one position to another
         * 
         */
        struct LondonController 
            {
            LondonController ()
                {
                //Define walker, with a 5x5 Gauss matrix
                Walker ww(5,5); w = &ww;

                // Seed the random walk algorithm
                const char* filename = "city_160.png";
                seed = w->seed(filename);

                map = astar.generateMap(64, 48); 
                
                // Set som resource fields
                map.at(20)[20] = 7; map.at(5)[10] = 7; map.at(10)[5] = 7; map.at(15)[5] = 7; 
                map.at(10)[15] = 7; map.at(10)[10] = 7; map.at(30)[20] = 7;
                map.at(30)[30] = 7; map.at(60)[40] = 7; map.at(25)[45] = 7;
                
                // Set values which is not traversable
                notTraversable.push_back(3);
                };
            
            /* Objects within list is drawn & progressed,
             * within repaint & turn.
             *
             * The list is used as input to repaint &
             * turn.
             */
            std::vector<LondonObject*> lObjects;

            /* (cols, rows) */
            std::vector<std::vector<int> > map;

            /* Position within map,
             * which is occupied by a LondonObject
             */
            std::map<Pos, LondonObject> occupied;

            /* Random walk - seed */
            std::vector<std::vector<double> > seed;

            std::vector<int> notTraversable;

            Walker *w;

            Astar astar;

            std::mutex lmutex;

            } static lController;

        /**
         * Parent class to all ingame entities.
         *
         * Contains:
         * Get Map        - Global map at lController
         * Set Map        - Global map at lController
         * Columns        - column count within map
         * Rows           - row count of column 0 within map
         *
         * Movement       - shortest path
         * Movement       - random walk
         * Position       - (x, y) 
         *
         * Virtual cpaint - painting object
         * Virtual turn   - action upon a new turn for that entity 
         */
        class LondonObject 
            { 
        /**
         * Reference to lControllers global map
         */
        std::vector<std::vector<int> > *map = &lController.map;
        
        //std::mutex lmutex;
        protected:
            int step;
            int x, y;
            int ID = 0;

            std::vector<Node> path;
        public:
            //LondonObject() {}; 
            
            virtual ~LondonObject() {};
            
            /**
             * This method is called from within,
             * London::Graphics::repaint().
             * 
             * It should paint what ever this
             * LondonObject should look like.
             */
            virtual void cpaint(HDC &hdc, RECT &rcWindow) {}; 

            /**
             * This method is called from within,
             * London::Graphics::turn().
             *
             * It should describe how this
             * LondonObject is changed.
             * 
             */
            virtual void turn() {};
            
            /** 
             * @return - number of columns in map, if none then -1
             */
            int getCols();

            /**
             * @return - number of rows at column 0, if no columns or no rows then -1
             */
            int getRows();
            
            std::vector<London::Pos> getNeighbourhood(int view);
            
            /**
             * Occupation of fields within a lController.map.
             * These function does not affect randomWalk or destination,
             * It's up to an entity wether it will use the information or not.
             */
            bool LockOrUnlcokOccupation(int x, int y);

            bool LockOrUnlcokOccupationIfValue(int x, int y, int v);

            // HELPER FUNCTIONS //
            bool isOccupied(int x, int y);
            
            bool occupy(int x, int y);

            bool releaseOccupation(int x, int y);
            // HELPER FUNCTIONS //

            LondonObject GetObjectAtOccupation(int x, int y);

            
            void pretty_print(std::string message);
            
            /** 
             * Change value of global map,
             * lController.
             */
            bool setMapValue(int x, int y, int value);

            /**
             * Get value from global map,
             * lController
             */
            int getMapValue(int x, int y);

            /**
             * This method is called from within,
             * London::Graphics::LondonObject.turn()
             *
             * Iterate one step in path[step],  
             * @update x = path[step].x
             * @update y = path[step].y
             * @update step++
             */
            void move();

            /**
             * Set x & y position
             * 
             * @x - position x
             * @y - position y 
             */
            void origin(int x, int y);
            
            /**
             * This method should fill path[],
             * with nodes from position (x,y),
             * to position (dx, dy)
             * 
             * Path is fund with help from Bellman-ford
             *
             * @dx - final x position
             * @dy - final y - position
             * @calculatePath - if false, set dx, dy & and set path[0] = (x, y)
             */
            void destination(int dx, int dy, bool calculatePath=TRUE);

            /**
             * This method fills path[],
             * with nodes starting at position (x,y).
             *
             * Nodes are generated by lController.Walker->randomWalk()
             * see Walker.h for more information.
             */
            std::vector<Node> randomWalk(); 

            /**
             * Random walk with Bernoulli.
             *
             * This method fills path[].
             * 
             * Using time as seed, choose,
             * direction, dx & dy. 
             *
             * Calculate full path with
             * Bellman-Ford and his
             * shortest_path(x, y, dx, dy).
             *
             * https://www.encyclopediaofmath.org/index.php/Bernoulli_random_walk
             */
            void randomWalkBernoulli();    

            int dx, dy;
            };
                
        /**
         * Repaint a window through double buffering.
         * 
         * This function calls cpaint on all elements,
         * within llObjects.
         *
         * https://msdn.microsoft.com/en-us/library/ms969905
         */
        void repaint(HWND hwnd, std::vector<LondonObject*> &llObjects);

        /**
         * This function calls turn on all elements,
         * within llObjects. 
         */
        void turn(std::vector<LondonObject*> &llObjects);
        };
        
    };

#endif