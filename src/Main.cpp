#include "Main.hpp"

#include "LatinSquareGenerator.hpp"
#include "LatinSquarePrintingUtils.hpp"
#include "LatinSquareTransversalGenerator.hpp"

int main() {
    auto latinSquareGenerator = LatinSquareGenerator::LatinSquareGenerator();
    auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(5);
    printLatinSquare(latinSquare);
    auto latinSquareTransversalGenerator = LatinSquareGenerator::LatinSquareTransversalGenerator();
    const auto transversal = latinSquareTransversalGenerator.findRandomTransversal(latinSquare);

    return 0;
}

// TODO:
// - add function to measure time of generating random latin square and random transversal
// - add function for printing transversal
// - consider moving files to correct folders (latin_square, transversal, etc.) and functions to correct files
// - check naming convension - idea: shorter names of variables if possible
