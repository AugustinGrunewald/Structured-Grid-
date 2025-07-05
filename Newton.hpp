#ifndef NEWTON_HPP
#define NEWTON_HPP

#include <functional>
#include "ASI.hpp"

inline double newton(std::function<double(double)> gammaPrimeNorm, double sTilde, double tol){
    double s1 = ASI(gammaPrimeNorm, 0, 1, 1e-4);
    int maxIterations = 0;

    // Initialization 
    double t = sTilde;
    double tNext = t - (ASI(gammaPrimeNorm, 0, t, 1e-4) - sTilde * s1) / gammaPrimeNorm(t);

    while (abs(tNext - t) >= tol && maxIterations <= 1e4){
        t = tNext;
        tNext = t - (ASI(gammaPrimeNorm, 0, t, 1e-4) - sTilde * s1) / gammaPrimeNorm(t);
        maxIterations++;
    }
    return tNext;
}

#endif