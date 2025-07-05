#ifndef ASI_HPP
#define ASI_HPP

#include <functional>
#include <iostream>
#include <cmath>

inline double I(std::function<double(double)> f, double a, double b){
    return (b - a)/6 * (f(a) + 4*f((a+b)/2) + f(b));
}

inline double I2(std::function<double(double)> f, double a, double b){
    double gamma = (a + b)/2;
    return I(f, a, gamma) + I(f, gamma, b);
}

inline double ASI(std::function<double(double)> f, double a, double b, double tol){
    // Following the ASI algorithm
    double i_1 = I(f, a, b);
    double i_2 = I2(f, a, b);
    double errest = abs(i_1 - i_2);

    if (errest < 15*tol){
        return i_2;
    } else {
        double gamma = (a + b)/2;
        return ASI(f, a, gamma, tol/2) + ASI(f, gamma, b, tol/2);
    }
}

#endif