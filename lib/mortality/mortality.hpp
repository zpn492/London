#ifndef __MORTALITY__
#define __MORTALITY__

    #include "../../SimpleJust.h"

namespace liv1 {
    /* Gompertz-Makeham parameters */
    #define MALE_DEATH_ALPHA 0.0005               
    #define MALE_DEATH_BETA 0.00007586
    #define MALE_DEATH_GAMMA log(1.09144)

    #define MALE_DISABILITY_ALPHA 0.0004               
    #define MALE_DISABILITY_BETA 0.00003467
    #define MALE_DISABILITY_GAMMA log(1.14815)  

    double mu(int age);
    
    double lx(int age);
    
    double npx(int termin_year, int age);
};
#endif