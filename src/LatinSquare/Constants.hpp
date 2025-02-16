#pragma once

#include <cstdint>

namespace LatinSquare {
    static constexpr uint_fast8_t EMPTY = 0xB0;
    static constexpr uint_fast8_t ROW_FLAG = 0x40;
    static constexpr uint_fast8_t COLUMN_FLAG = 0x80;

    enum class Type : uint_fast8_t {
        Normal = 0x00,
        Reduced = 0x01,
        ReducedCyclic = 0x02,
        ReducedDiagonal = 0x03,
        ReducedSuperSymmetric = 0x04
    };
}
