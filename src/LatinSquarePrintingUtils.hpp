#pragma once

#include <functional>
#include <vector>

#include "LatinSquare.hpp"
#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    void printLatinSquareFullIds(LatinSquare& latinSquare);
    void printLatinSquareBoard(LatinSquare& latinSquare);
    void sortTransversal(std::vector<std::reference_wrapper<Cell>>& transversal);
    void printTransversalFullIds(std::vector<std::reference_wrapper<Cell>>& transversal);
}
