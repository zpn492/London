#ifndef __CALCULUS__
#define __CALCULUS__

#include "../../SimpleJust.h"

class CFunction {
public:
    
    virtual double f(double) { return 0.0; };

    /* Limit of Riemann sums */
    double integrate(CFunction &c, int a, int b);

    /* Forward differential approximation Euler 
     * f'(x) = ( f(x + h) - f(x) ) / h
     * 
     * h is estimated to DBL_MIN * 2^973
     * DBL_MIN is smallest positiv floating-point 1E-37 or smaller
     */
    double differentiate(CFunction &c, int x);

};

#endif