#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Constants.hpp"
#include "LatinSquare.hpp"

namespace LatinSquare {
    [[nodiscard]] Type convert(const std::string& type) noexcept;
    [[nodiscard]] const std::vector<uint_fast8_t> convert(
        const std::string& filename, const uint_fast8_t size) noexcept;
    void printIds(const LatinSquare& latinSquare) noexcept;
    void printBoard(const LatinSquare& latinSquare) noexcept;
    void printFile(const LatinSquare& latinSquare, const std::string filename) noexcept;
}
