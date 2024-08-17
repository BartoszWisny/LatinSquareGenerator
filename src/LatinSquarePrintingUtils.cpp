#include "LatinSquarePrintingUtils.hpp"

#include <iostream>

#include "LatinSquare.hpp"

namespace LatinSquareGenerator {
    void printLatinSquare(LatinSquare& latinSquare) {
        const auto& grid = latinSquare.getGrid();

        for (const auto& cell : grid) {
            std::cout << cell.getFullId() << std::endl;
        }
    }

    // TODO: add more functions for printing latin squares like: print "board", etc.
}
