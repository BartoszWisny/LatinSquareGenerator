#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

#include "Constants.hpp"
#include "LatinSquare.hpp"
#include "SymmetricLatinSquare.hpp"

namespace LatinSquare {
    [[nodiscard]] Type getType(const std::string& type) noexcept;

    [[nodiscard]] const std::pair<uint_fast8_t, std::vector<uint_fast8_t>> convert(
        const std::string& filename) noexcept;

    void printBoard(const LatinSquare& latinSquare) noexcept;
    void printFile(const LatinSquare& latinSquare, const std::string filename) noexcept;

    [[nodiscard]] const std::pair<uint_fast8_t, std::vector<uint_fast8_t>> symmetricConvert(
        const std::string& filename) noexcept;

    void printBoard(const SymmetricLatinSquare& symmetricLatinSquare) noexcept;
    void printTriangularBoard(const SymmetricLatinSquare& symmetricLatinSquare) noexcept;
    void printFile(const SymmetricLatinSquare& symmetricLatinSquare, const std::string filename) noexcept;
    void printTriangularFile(const SymmetricLatinSquare& symmetricLatinSquare, const std::string filename) noexcept;
}
