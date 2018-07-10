#include "calculus.hpp"

double CFunction::integrate(CFunction &c, int a, int b) {
        double fi = 0.0, delta_i = 0.0;
        for(int i = a; i <= b; i++) fi += c.f(i);
        delta_i = (b - a)*1.0 / (b - a + 1);
        return fi * delta_i;
    };

double CFunction::differentiate(CFunction &c, int x) {
        int timer = 2, counter = 2;
        double value = 0;
        while(value == 0) {
            value = (c.f(x + DBL_MIN * pow(timer, counter)) - c.f(x) ) / (DBL_MIN * pow(timer, counter));
            counter++;
        }
        return value;
    };