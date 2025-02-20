#pragma once

#include <cstdint>

namespace LatinSquare {
    static constexpr uint_fast8_t EMPTY = 0xFF;

    enum class Type : uint_fast8_t {
        Normal = 0x00,
        Reduced = 0x01,
        ReducedCyclic = 0x02,
        ReducedDiagonal = 0x03,
        ReducedSuperSymmetric = 0x04
    };
}
