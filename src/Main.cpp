#include "Main.hpp"

#include "LatinSquareGenerator.hpp"
#include "LatinSquarePrintingUtils.hpp"

int main() {
    auto latinSquareGenerator = LatinSquareGenerator::LatinSquareGenerator();
    auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(5);
    printLatinSquare(latinSquare);

    return 0;
}

// TODO:
// - add function to measure time of generating random latin square
