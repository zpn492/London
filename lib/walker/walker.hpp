#ifndef __WALKER__
#define __WALKER__

#include "../../SimpleJust.h"

#include "../../lib/matrix/matrix.hpp"
#include "../../lib/astar/astar.hpp"
#include "../../lib/png/png.h"

class Walker
    {
    int cols, rows;
    std::vector<std::vector<double> > filter, filter2;
    
protected:
    /**
     * Convolve single pixel
     */
    double convolveSinglePixel(std::vector<std::vector<double> > &a, 
        std::vector<std::vector<double> > &b,
        int x,
        int y);
    
    /**
     * This function is used to smoothing an image thus reducing noise
     * @a - an image, 
     *
     * https://en.wikipedia.org/wiki/Convolution#Discrete_convolution
     */
    std::vector<std::vector<double> > convolve2D(std::vector<std::vector<double> > a, 
        std::vector<std::vector<double> > b, bool debug=0);

    
    /**
     * Load a png file.
     * Store red in the a 2-dimensional vector.
     *
     * uses lib/png/png.h
     *
     * @return - 2-dimensional vector, with values from 0-255
     */
    std::vector<std::vector<double> > decodeTwoSteps(const char* filename);
    
    /**
     * Change all values to either 0 or 255,
     * this depending on the existing value.
     */
    void applyThreshold(std::vector<std::vector<double> > &data);
    
    /**
     * Foreach position(x, y) look at neighbour values,
     * depending on neighbourhood values, align all values.
     */
    void alignNeighbours(std::vector<std::vector<double> > &data, int thres);

    /**
     * Set edge
     */
    void applyEdge(std::vector<std::vector<double> > &data);

    /**
     * DoG (Difference of Gaussian)
     */
    std::vector<std::vector<double> > DoG();

public:
    Walker(int cols, int rows);
    /**
     * Normal distribution (or Gaussian, Gauss or Laplace-Gauss) 
     *
     * This method generates a 2-dimensional vector with values,
     * from 0 to 1. High values are to be fund in the middel,
     * as you move towards edges values decrease.
     *
     * https://en.wikipedia.org/wiki/Normal_distribution
     */ 
    std::vector<std::vector<double> > generateGauss(int sigma, int k=1);

    /**
     * Seed
     */
    std::vector<std::vector<double> > seed(const char *filename);

    /**
     * Random walk
     *
     * @return - random generated from filename
     * 
     * https://en.wikipedia.org/wiki/Random_walk#Gaussian_random_walk
     */
     std::vector<Node> randomWalk(std::vector<std::vector<double> > &data);
    };

#endif