#include "Main.hpp"

#include "LatinSquare.hpp"
#include "LatinSquareGenerator.hpp"
#include "LatinSquarePrintingUtils.hpp"

int main() {
    auto latinSquareGenerator = LatinSquareGenerator::LatinSquareGenerator();
    auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(5);
    printLatinSquare(latinSquare);

    return 0;
}

// TODO:
// - add refs where possible
// - add const where possible (mainly after function names)
// ...
