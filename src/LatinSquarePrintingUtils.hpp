#pragma once

#include <functional>
#include <vector>

#include "LatinSquare.hpp"
#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    void printLatinSquare(LatinSquare& latinSquare);
    void sortTransversal(std::vector<std::reference_wrapper<Cell>>& transversal);
    void printTransversal(std::vector<std::reference_wrapper<Cell>>& transversal);
}
