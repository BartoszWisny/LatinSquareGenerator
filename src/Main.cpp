#include "Main.hpp"

#include "LatinSquare/Generator.hpp"
#include "LatinSquare/Utils.hpp"
#include "Transversal/Generator.hpp"
#include "Transversal/Utils.hpp"

int main() {
    auto latinSquareGenerator = LatinSquare::Generator();
    auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(5, true);
    latinSquare.sortGrid();
    // LatinSquare::printLatinSquareFullIds(latinSquare);
    // LatinSquare::printLatinSquareBoard(latinSquare);

    latinSquare.setRegions();
    auto transversalGenerator = Transversal::Generator();
    auto transversal = transversalGenerator.findRandomTransversal(latinSquare);
    // Transversal::printTransversalFullIds(transversal);
    Transversal::printTransversalBoard(latinSquare, transversal);

    return 0;
}

// TODO:
// - add function to measure time of generating random latin square and random transversal
// - consider moving functions to correct files
// - consider using back_emplacer instead of back_iterator
// - optimize code where possible
// - make functions inline if possible
