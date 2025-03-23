#include "Utils.hpp"

#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

#include <cpp/string.hpp>

namespace LatinSquare {
    Type getType(const std::string& type) noexcept {
        if (type == TYPE_NORMAL) {
            return Type::Normal;
        } else if (type == TYPE_REDUCED) {
            return Type::Reduced;
        } else if (type == TYPE_REDUCED_CYCLIC) {
            return Type::ReducedCyclic;
        } else if (type == TYPE_REDUCED_DIAGONAL) {
            return Type::ReducedDiagonal;
        } else if (type == TYPE_REDUCED_SUPER_SYMMETRIC) {
            return Type::ReducedSuperSymmetric;
        } else {
            return Type::Unknown;
        }
    }

    const std::pair<uint_fast8_t, std::vector<uint_fast8_t>> convert(const std::string& filename) noexcept {
        std::ifstream file(filename);

        if (!file) {
            return {};
        }

        std::vector<uint_fast8_t> numbers;
        std::string line, word;
        uint_fast8_t number, size = 0, counter = 0, linesCounter = 0;

        while (std::getline(file, line)) {
            std::istringstream iss(line);

            while (iss >> word) {
                number = std::strtoul(word.c_str(), nullptr, 10);

                if (number <= MAX_SIZE) {
                    numbers.emplace_back(--number);
                    ++counter;
                } else {
                    return {};
                }
            }

            if (size == 0) {
                size = counter;
            } else if (counter != size) {
                return {};
            }

            counter = 0;
            ++linesCounter;
        }

        if (linesCounter != size) {
            return {};
        }

        return {size, numbers};
    }

    void printIds(const LatinSquare& latinSquare) noexcept {
        const auto& grid = latinSquare.grid();

        for (const auto& cell : grid) {
            std::cout.write(cell->id().c_str(), cell->id().size());
            std::cout.put('\n');
        }
    }

    void printBoard(const LatinSquare& latinSquare) noexcept {
        const auto& grid = latinSquare.grid();
        std::string leftBar = "+", repeatedLeftBar, spaces, numberString;
        leftBar.append(std::string(static_cast<uint_fast8_t>(std::log10(latinSquare.size()) + 1) + 2, '-'));
        repeatedLeftBar = cpp::repeat(leftBar, latinSquare.size());
        uint8_t number;

        for (const auto& cell : grid) {
            if (cell->rawColumn() == 0) {
                std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                std::cout.put('+');
                std::cout.put('\n');
            }

            number = cell->number();
            spaces = std::string(static_cast<uint_fast8_t>(std::log10(latinSquare.size()))
                                 - static_cast<uint_fast8_t>(std::log10(++number)) + 1, ' ');
            numberString = std::to_string(number);
            std::cout.put('|');
            std::cout.write(spaces.c_str(), spaces.size());
            std::cout.write(numberString.c_str(), numberString.size());
            std::cout.put(' ');

            if (cell->rawColumn() == latinSquare.size() - 1) {
                std::cout.put('|');
                std::cout.put('\n');
            }
        }

        std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
        std::cout.put('+');
        std::cout.put('\n');
    }

    void printFile(const LatinSquare& latinSquare, const std::string filename) noexcept {
        uint_fast16_t gridSize = latinSquare.size();
        gridSize *= latinSquare.size();
        const auto& grid = latinSquare.grid();
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;
        std::string number;
        std::ofstream file(filename);

        if (!file.is_open()) {
            return;
        }

        for (uint_fast16_t index = 0; index < gridSize; ++index) {
            number = std::to_string(grid[index]->number() + 1);
            number += " ";
            file.write(number.c_str(), number.size());

            if (++column == latinSquare.size()) {
                column = 0;
                ++row;
                file.put('\n');
            }
        }
    }

    const std::pair<uint_fast8_t, std::vector<uint_fast8_t>> symmetricConvert(const std::string& filename) noexcept {
        std::ifstream file(filename);

        if (!file) {
            return {};
        }

        std::vector<uint_fast8_t> numbers;
        std::string line, word;
        uint_fast8_t number, size = 1, counter = 0, linesCounter = 0;

        while (std::getline(file, line)) {
            std::istringstream iss(line);

            while (iss >> word) {
                number = std::strtoul(word.c_str(), nullptr, 10);

                if (number <= MAX_SIZE) {
                    numbers.emplace_back(--number);
                    ++counter;
                } else {
                    return {};
                }
            }

            if (counter != size) {
                return {};
            }

            counter = 0;
            ++linesCounter;
            ++size;
        }

        if (linesCounter != --size) {
            return {};
        }

        return {size, numbers};
    }

    void printIds(const SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        const auto& grid = symmetricLatinSquare.grid();

        for (const auto& cell : grid) {
            std::cout.write(cell->id().c_str(), cell->id().size());
            std::cout.put('\n');
        }
    }

    void printBoard(const SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        const auto& grid = symmetricLatinSquare.grid();
        std::string leftBar = "+", repeatedLeftBar, spaces, numberString;
        leftBar.append(std::string(static_cast<uint_fast8_t>(std::log10(symmetricLatinSquare.size()) + 1) + 2, '-'));
        repeatedLeftBar = cpp::repeat(leftBar, symmetricLatinSquare.size());
        uint8_t number;

        for (const auto& cell : grid) {
            if (cell->rawColumn() == 0) {
                std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
                std::cout.put('+');
                std::cout.put('\n');
            }

            number = cell->number();
            spaces = std::string(static_cast<uint_fast8_t>(std::log10(symmetricLatinSquare.size()))
                                 - static_cast<uint_fast8_t>(std::log10(++number)) + 1, ' ');
            numberString = std::to_string(number);
            std::cout.put('|');
            std::cout.write(spaces.c_str(), spaces.size());
            std::cout.write(numberString.c_str(), numberString.size());
            std::cout.put(' ');

            if (cell->rawColumn() == symmetricLatinSquare.size() - 1) {
                std::cout.put('|');
                std::cout.put('\n');
            }
        }

        std::cout.write(repeatedLeftBar.c_str(), repeatedLeftBar.size());
        std::cout.put('+');
        std::cout.put('\n');
    }

    void printFile(const SymmetricLatinSquare& symmetricLatinSquare, const std::string filename) noexcept {
        uint_fast16_t gridSize = symmetricLatinSquare.size();
        gridSize *= symmetricLatinSquare.size();
        const auto& grid = symmetricLatinSquare.grid();
        uint_fast16_t row = 0;
        uint_fast16_t column = 0;
        std::string number;
        std::ofstream file(filename);

        if (!file.is_open()) {
            return;
        }

        for (uint_fast16_t index = 0; index < gridSize; ++index) {
            number = std::to_string(grid[index]->number() + 1);
            number += " ";
            file.write(number.c_str(), number.size());

            if (++column == symmetricLatinSquare.size()) {
                column = 0;
                ++row;
                file.put('\n');
            }
        }
    }
}
