#include "Main.hpp"

#include "LatinSquareGenerator.hpp"
#include "LatinSquarePrintingUtils.hpp"
#include "LatinSquareTransversalGenerator.hpp"

int main() {
    auto latinSquareGenerator = LatinSquareGenerator::LatinSquareGenerator();
    auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(5, true);
    printLatinSquareFullIds(latinSquare);
    printLatinSquareBoard(latinSquare);

    latinSquare.setRegions();
    auto transversalGenerator = LatinSquareGenerator::LatinSquareTransversalGenerator();
    auto transversal = transversalGenerator.findRandomTransversal(latinSquare);
    printTransversalFullIds(transversal);

    return 0;
}

// TODO:
// - add function to measure time of generating random latin square and random transversal
// - consider moving files to correct folders (latin_square, transversal, etc.) and functions to correct files
// - check naming convension - idea: shorter names of variables if possible
