#pragma once

#include <cstdint>
#include <vector>

#include "LatinSquare/LatinSquare.hpp"

namespace Transversal {
    void printIds(LatinSquare::LatinSquare& latinSquare, std::vector<uint_fast16_t>& transversal) noexcept;
    void printBoard(LatinSquare::LatinSquare& latinSquare, std::vector<uint_fast16_t>& transversal) noexcept;
}
