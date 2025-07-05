#include "grid.hpp"

// -- Algebraic Grid Generation implementation --
void Domain::algebraicGridGeneration(const std::string xPath, const std::string yPath, const std::size_t n) const{
    // Setting timer
    sf::Timer timer;
    timer.start();

    if (n <= 2){
        throw std::invalid_argument("Input n should be strictly greater than 2.");
    }

    // Initializing the matrices
    std::vector<double> xMatrix((n+1)*(n+1));
    std::vector<double> yMatrix((n+1)*(n+1));

    // Defining the projectors Pxi, PEta and the tensors product, sum
    auto Pxi = [this](double xi, double eta){
        return (1 - xi) * sides[SIDE_LEFT]->at(eta) + xi * sides[SIDE_RIGHT]->at(eta);
    };
    auto PEta = [this](double xi, double eta){
        return (1 - eta) * sides[SIDE_BOTTOM]->at(xi) + eta * sides[SIDE_TOP]->at(xi);
    };
    auto product = [this](double xi, double eta){
        return (1 - xi) * (1 - eta) * sides[SIDE_BOTTOM]->at(0) + (1 - xi) * eta * sides[SIDE_LEFT]->at(1) + xi * (1 - eta) * sides[SIDE_RIGHT]->at(0) + xi * eta * sides[SIDE_TOP]->at(1);
    };
    auto sum = [Pxi, PEta, product](double xi, double eta){
        return Pxi(xi, eta) + PEta(xi, eta) - product(xi, eta);
    };
    
    // Filling the matrices with the right values
    double h = 1.0 / n;
    for (std::size_t i = 0; i <= n; i++){
        for (std::size_t j = 0; j <= n; j++){
            Point gridPoint = sum(i * h, j * h);
            xMatrix[i * (n+1) + j] = gridPoint.x;
            yMatrix[i * (n+1) + j] = gridPoint.y;
        }
    }

    // Stopping timer
    timer.stop();

    // Writing the text files
    txtWriter(xPath, xMatrix, n+1);
    txtWriter(yPath, yMatrix, n+1);
}


// -- Write the matrix in a text file --
void Domain::txtWriter(const std::string path, const std::vector<double>& pointMatrix, const std::size_t n) const {
    std::ofstream file(path);

    for (std::size_t i = 0; i < n*n; i++){
        file << pointMatrix[i];
        if (i + 1 != n*n) ((i + 1) % n == 0)? file << "\n" : file << " ";
    }
}


// -- Equation curve implementation --
Point EquationCurve::at(double s) const {
    // Value already computed
    auto key = cache.find(s);
    if (key != cache.end()) return key->second;
    else {
        Point result = gamma(newton([this](double t){return gammaPrimeNorm(t);}, s, 1e-4));
        cache[s] = result;
        return result;
    }
}

// -- VERSION WITHOUT CACHE --
// Point EquationCurve::at(double s) const {
//     return gamma(newton([this](double t){return gammaPrimeNorm(t);}, s, 1e-4));
// }

double EquationCurve::arcLength(double t) const {
    return ASI([this](double x){return gammaPrimeNorm(x);}, 0, t, 1e-4);
}

Point BottomCurve::gamma(double t) const {
    auto g = [](double x){
        return (x < -3) ? 1 + exp(-3 * (x + 6)) : 1 + exp(3 * x);
    };

    double x = (1 - t) * (-10) + 5 * t;
    double y =  1.0 / (2 * g(x));

    return Point(x,y);
}

Point BottomCurve::gammaPrime(double t) const {
    auto g = [](double x){
        return (x < -3) ? 1 + exp(-3 * (x + 6)) : 1 + exp(3 * x);
    };
    auto gPrime = [](double x){
        return (x < -3) ? -3 * exp(-3 * (x + 6)) : 3 * exp(3 * x);
    };

    double x = (1 - t) * (-10) + 5 * t;
    
    double xPrime = 15;
    double yPrime = - gPrime(x) * xPrime / (2 * pow(g(x),2));

    return Point(xPrime,yPrime);
}