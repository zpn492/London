#include "insurance.hpp"

namespace liv1 {

    double Kontinuert::f(double i) 
        {
        return interest + mu;
        };
    
    Kontinuert::Kontinuert(double mu, double interest) 
        { 
        this->mu = mu; this->interest = log(1.0 + interest) ; 
        };

    /* e^(- integrate(f, a, b)), b > a  */
    double diskontering(Kontinuert k, int a, int b) 
        {
        return pow(E, -k.integrate(k, a, b));
        };

    /* e^( integrate(f, a, b)), b > a  */
    double kontering(Kontinuert k, int a, int b) 
        {
        return pow(E, k.integrate(k, a, b));
        };
};