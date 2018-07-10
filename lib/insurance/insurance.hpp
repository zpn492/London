#ifndef __INSURANCE__
#define __INSURANCE__

/* Standard includes */
#include "../../SimpleJust.h"

/* Homemade files */
#include "../mortality/mortality.hpp"
#include "../calculus/calculus.hpp"

namespace liv1 {
   
    class Kontinuert : public CFunction 
        {
        
        /**
         * Death intensity
         * Ex. Gompertz-Makehame, liv1::mu(int age) from "mortality.hpp"
         */
        double mu;

        /**
         * Force of interest
         * Ex. ln ( 1 + 0.045 ), for 4,5 % interest
         */
        double interest;
        
        double f(double i);
    
    public:
        Kontinuert(double mu, double interest);
        };

    /* e^(- integrate(f, a, b)), b > a  */
    double diskontering(Kontinuert k, int a, int b);

    /* e^( integrate(f, a, b)), b > a  */
    double kontering(Kontinuert k, int a, int b);
    
    /* Approximate integration
     * By Limit Riemann sums 
     */
    double intensity_of_death(int a, int b, int age);
    
    /**
     * Discounting with a constant force of interest
     */
    double discounting(int n, double interest);
    
    /**
     * Pure endowment
     * An n-year pure (life) endowment of 1 is a unit,
     * that is paid at the end of n-years, if he/she is still alive
     */
    class Endowment
        {       
    public:
        /* Present value */
        double pv(int Deferred=0, int a=0, int b=1, int age=30);
        
        /* Expected present value */
        //double epv(int Deferred=0, int n, int age);
        };

    class Annuity
        {
        Annuity(double periodBenefit, int period);

        double periodBenefit;
        double period;
        };
    
    class Cashflow
        {
        /* Benefits */
        
        double bn; /* Sum insured if survives to age n */

        double bd; /* Sum insured upon death at time t */

        Annuity bt; /* running benefits starting af time t and stops at time m */

        /* Premiums */
        double a0; /* Deposit at time 0 */

        double at; /* Running premium */
        };
    
    
};

#endif