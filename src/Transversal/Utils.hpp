#pragma once

#include <functional>
#include <set>
#include <string>
#include <vector>

#include "LatinSquare/Cell.hpp"
#include "LatinSquare/LatinSquare.hpp"

namespace Transversal {
    void sortTransversal(std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal);

    void printTransversalFullIds(std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal);
    bool checkIfTransversalCell(const LatinSquare::Cell& cell, const std::set<std::string>& ids);
    void printTransversalBoard(
        LatinSquare::LatinSquare& latinSquare, std::vector<std::reference_wrapper<LatinSquare::Cell>>& transversal);
}
