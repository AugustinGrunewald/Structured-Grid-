#ifndef GRID_HPP
#define GRID_HPP

#include <memory> 
#include <string>
#include <vector>
#include <fstream>
#include "ASI.hpp"
#include "Newton.hpp"
#include "timer.hpp"

// -- Class Point --
class Point {
public:
    double x;
    double y;

    Point(): x(0.0), y(0.0){};
    Point(double xCoord, double yCoord): x(xCoord), y(yCoord){};

    Point operator+(const Point& rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator-(const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
};

inline Point operator*(double c, const Point& pt){
    return Point(c * pt.x, c * pt.y);
}


// -- Class Curve and Subclasses --
class Curve {
public:
    virtual ~Curve() = default;
    virtual Point at(double s) const = 0;
};

class StraightLine : public Curve {
public:
    StraightLine(): start(Point()), end(Point()){};
    StraightLine(Point startingPoint, Point endingPoint): start(startingPoint), end(endingPoint){};

    Point at(double s) const override {
        return Point((1-s) * start.x + s * end.x, (1-s) * start.y + s * end.y);
    }

private:
    Point start;
    Point end;
};

class EquationCurve : public Curve {
public:
    virtual ~EquationCurve() = default;
    Point at(double s) const override;
    double arcLength(double t) const;

private:
    virtual Point gamma(double t) const = 0;
    virtual Point gammaPrime(double t) const = 0;
    double gammaPrimeNorm(double t) const {
        return sqrt(pow(gammaPrime(t).x,2) + pow(gammaPrime(t).y,2));
    }

    mutable std::unordered_map<double, Point> cache;
};

class BottomCurve : public EquationCurve {
private:
    Point gamma(double t) const override;
    Point gammaPrime(double t) const override;
};


// -- Class Domain --
class Domain {
public:
    static constexpr int SIDE_BOTTOM = 0;
    static constexpr int SIDE_LEFT = 1;
    static constexpr int SIDE_TOP = 2;
    static constexpr int SIDE_RIGHT = 3;

    Domain(bool withBottomBoundary){
        if (withBottomBoundary) sides[SIDE_BOTTOM] = std::make_unique<BottomCurve>();
        else sides[SIDE_BOTTOM] = std::make_unique<StraightLine>(Point(-10,0), Point(5,0)); 

        sides[SIDE_LEFT] = std::make_unique<StraightLine>(Point(-10,0), Point(-10,3)); 
        sides[SIDE_TOP] = std::make_unique<StraightLine>(Point(-10,3), Point(5,3));
        sides[SIDE_RIGHT] = std::make_unique<StraightLine>(Point(5,0), Point(5, 3)); 
    }

    void algebraicGridGeneration(const std::string xPath, const std::string yPath, const std::size_t n) const;
    void txtWriter(const std::string path, const std::vector<double>& pointMatrix, const std::size_t n) const;

private:
    std::array<std::unique_ptr<Curve>, 4> sides; // bottom, left, top, right
};

#endif