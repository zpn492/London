#include "mortality.hpp"
namespace liv1 {
    double mu(
        int age
    )
        {
        return MALE_DEATH_ALPHA + MALE_DEATH_BETA * pow(E, MALE_DEATH_GAMMA*age);
        };
    
    double lx(
        int age
    )
        {
        // exp(-alpha * t - beta * (exp(gamma * t) - 1.0) / gamma)
        return pow(E, -MALE_DEATH_ALPHA * age - MALE_DEATH_BETA * (pow(E, MALE_DEATH_GAMMA*age) - 1.0 ) / MALE_DEATH_GAMMA );
        };
    
    double npx(
        int termin_year,
        int age
    )
        {
        return lx(age + termin_year) / lx(age);
        };
};