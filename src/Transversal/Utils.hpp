#pragma once

#include <cstdint>
#include <vector>

#include "LatinSquare/LatinSquare.hpp"
#include "LatinSquare/SymmetricLatinSquare.hpp"

namespace Transversal {
    void printBoard(LatinSquare::LatinSquare& latinSquare, std::vector<uint_fast16_t>& transversal) noexcept;

    void printBoard(
        LatinSquare::SymmetricLatinSquare& symmetricLatinSquare, std::vector<uint_fast16_t>& transversal) noexcept;
    // void printTriangularBoard(
    //     LatinSquare::SymmetricLatinSquare& symmetricLatinSquare, std::vector<uint_fast16_t>& transversal) noexcept;
}
