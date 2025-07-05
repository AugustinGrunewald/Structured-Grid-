#include "grid.hpp"

int main(){
    // -- DOMAIN WITH STRAIGHT LINES --
    try {
        Domain domain1(false);
        std::string xPath1 = "x1.txt";
        std::string yPath1 = "y1.txt";
        domain1.algebraicGridGeneration(xPath1, yPath1, 50);
    } catch (const std::invalid_argument& e){
        std::cerr << "Error in Domain creation : " << e.what() << std::endl;
    }

    // -- DOMAIN WITH BOTTOM CURVE --
    try {
        Domain domain2(true);
        std::string xPath2 = "x2.txt";
        std::string yPath2 = "y2.txt";
        domain2.algebraicGridGeneration(xPath2, yPath2, 50);
    } catch (const std::invalid_argument& e){
        std::cerr << "Error in Domain creation : " << e.what() << std::endl;
    }

    return 0;
}