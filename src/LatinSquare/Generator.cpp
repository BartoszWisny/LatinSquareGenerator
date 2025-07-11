#include "Generator.hpp"

// #include <iostream>

#include <cpp/random.hpp>

#include "Cell.hpp"
#include "EntropyData.hpp"

namespace LatinSquare {
    const LatinSquare Generator::random(const uint_fast8_t size, const Type type) noexcept {
        cpp::splitmix64 splitmix64;
        LatinSquare latinSquare(size, type, splitmix64);

        uint_fast16_t index = DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        EntropyData entropyData;

        updateHistory_.reserve(latinSquare.notFilled());
        backtrackingHistory_.reserve(latinSquare.notFilled());

        uint_fast16_t counter = 0;
        uint_fast32_t iterations = 0;

        while (latinSquare.notFilled()) {
            auto& cell = latinSquare.randomMinEntropyCell(index);

            if (cell.positiveEntropy()) {
                index = DEFAULT_CELL_INDEX;
                counter = 0;

                number = cell.numbers()[splitmix64.next() % cell.numbers().size()];
                entropyData = cell.entropyData();
                latinSquare.fillAndClear(cell, number);

                updateHistory_.emplace_back(cell.index(), number, entropyData, latinSquare.update(cell, number));

                if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                    backtrackingHistory_.emplace_back(cell.index(), entropyData);
                }
            } else {
                if (updateHistory_.empty()) {
                    break;
                }

                if (++counter > 1) {
                    latinSquare.set(backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                    backtrackingHistory_.pop_back();

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                index = updateHistory_.back().index();

                latinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (++iterations > MAX_ITERATIONS) {
                latinSquare.set(type);

                updateHistory_.clear();
                backtrackingHistory_.clear();
                iterations = 0;
            }
        }

        return latinSquare;
    }

    const LatinSquare Generator::random(const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers) noexcept {
        cpp::splitmix64 splitmix64;
        LatinSquare latinSquare(size, numbers, splitmix64);

        uint_fast16_t index = DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        EntropyData entropyData;

        updateHistory_.reserve(latinSquare.notFilled());
        backtrackingHistory_.reserve(latinSquare.notFilled());

        uint_fast16_t counter = 0;
        uint_fast32_t iterations = 0;

        while (latinSquare.notFilled()) {
            auto& cell = latinSquare.randomMinEntropyCell(index);

            if (cell.positiveEntropy()) {
                index = DEFAULT_CELL_INDEX;
                counter = 0;

                number = cell.numbers()[splitmix64.next() % cell.numbers().size()];
                entropyData = cell.entropyData();
                latinSquare.fillAndClear(cell, number);

                updateHistory_.emplace_back(cell.index(), number, entropyData, latinSquare.update(cell, number));

                if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                    backtrackingHistory_.emplace_back(cell.index(), entropyData);
                }
            } else {
                if (updateHistory_.empty()) {
                    break;
                }

                if (++counter > 1) {
                    latinSquare.set(backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                    backtrackingHistory_.pop_back();

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                index = updateHistory_.back().index();

                latinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (++iterations > MAX_ITERATIONS) {
                latinSquare.set(numbers);

                updateHistory_.clear();
                backtrackingHistory_.clear();
                iterations = 0;
            }
        }

        return latinSquare;
    }

    const boost::multiprecision::mpz_int Generator::count(const uint_fast8_t size, const Type type) noexcept {
        LatinSquare latinSquare(size, type);

        if (latinSquare.notFilled() < 2) {
            // std::cout << "Iterations: 1" << std::endl;

            return 1;
        }

        uint_fast16_t index = DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        EntropyData entropyData;

        updateHistory_.reserve(latinSquare.notFilled());
        backtrackingHistory_.reserve(latinSquare.notFilled());

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;
        // boost::multiprecision::mpz_int iterations = 0;

        while (true) {
            // ++iterations;

            if (latinSquare.notFilled() > 1) {
                auto& cell = latinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(cell.index(), number, entropyData, latinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (updateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        latinSquare.set(backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    index = updateHistory_.back().index();

                    latinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                    latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                counter = 1;
                latinSquaresCounter += latinSquare.lastNotFilledCell().entropy();

                index = updateHistory_.back().index();

                latinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        // std::cout << "Iterations: " << iterations << std::endl;

        return latinSquaresCounter;
    }

    const boost::multiprecision::mpz_int Generator::count(LatinSquare& latinSquare) noexcept {
        if (latinSquare.notFilled() < 2) {
            return 1;
        }

        uint_fast16_t index = DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        EntropyData entropyData;

        updateHistory_.reserve(latinSquare.notFilled());
        backtrackingHistory_.reserve(latinSquare.notFilled());

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled() > 1) {
                auto& cell = latinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(cell.index(), number, entropyData, latinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (updateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        latinSquare.set(backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    index = updateHistory_.back().index();

                    latinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                    latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                counter = 1;
                latinSquaresCounter += latinSquare.lastNotFilledCell().entropy();

                index = updateHistory_.back().index();

                latinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        return latinSquaresCounter;
    }

    const SymmetricLatinSquare Generator::symmetricRandom(const uint_fast8_t size, const Type type) noexcept {
        cpp::splitmix64 splitmix64;
        SymmetricLatinSquare symmetricLatinSquare(size, type, splitmix64);

        if (type == Type::ReducedDiagonal) {
            return symmetricLatinSquare;
        }

        uint_fast16_t index = DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        EntropyData entropyData;

        updateHistory_.reserve(symmetricLatinSquare.notFilled());
        backtrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        uint_fast16_t counter = 0;
        uint_fast32_t iterations = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.randomMinEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.numbers()[splitmix64.next() % cell.numbers().size()];
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (updateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        symmetricLatinSquare.set(
                            backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    index = updateHistory_.back().index();

                    symmetricLatinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else if (symmetricLatinSquare.checkDiagonal()) {
                symmetricLatinSquare.fillGrid();
                break;
            } else {
                counter = 1;

                index = updateHistory_.back().index();

                symmetricLatinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (++iterations > SYMMETRIC_MAX_ITERATIONS) {
                symmetricLatinSquare.set(type);

                updateHistory_.clear();
                backtrackingHistory_.clear();
                iterations = 0;
            }
        }

        return symmetricLatinSquare;
    }

    const SymmetricLatinSquare Generator::symmetricRandom(
        const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers) noexcept {
        cpp::splitmix64 splitmix64;
        SymmetricLatinSquare symmetricLatinSquare(size, numbers, splitmix64);

        uint_fast16_t index = DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        EntropyData entropyData;

        updateHistory_.reserve(symmetricLatinSquare.notFilled());
        backtrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        uint_fast16_t counter = 0;
        uint_fast32_t iterations = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.randomMinEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.numbers()[splitmix64.next() % cell.numbers().size()];
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (updateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        symmetricLatinSquare.set(
                            backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    index = updateHistory_.back().index();

                    symmetricLatinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else if (symmetricLatinSquare.checkDiagonal()) {
                symmetricLatinSquare.fillGrid();
                break;
            } else {
                counter = 1;

                index = updateHistory_.back().index();

                symmetricLatinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (++iterations > SYMMETRIC_MAX_ITERATIONS) {
                symmetricLatinSquare.set(numbers);

                updateHistory_.clear();
                backtrackingHistory_.clear();
                iterations = 0;
            }
        }

        return symmetricLatinSquare;
    }

    const boost::multiprecision::mpz_int Generator::symmetricCount(const uint_fast8_t size, const Type type) noexcept {
        SymmetricLatinSquare symmetricLatinSquare(size, type);

        if (type == Type::ReducedDiagonal) {
            // std::cout << "Iterations: 1" << std::endl;

            return 0;
        }

        if (!symmetricLatinSquare.notFilled()) {
            // std::cout << "Iterations: 1" << std::endl;

            return 1;
        }

        uint_fast16_t index = DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        EntropyData entropyData;

        updateHistory_.reserve(symmetricLatinSquare.notFilled());
        backtrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;
        // boost::multiprecision::mpz_int iterations = 0;

        while (true) {
            // ++iterations;

            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (updateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        symmetricLatinSquare.set(
                            backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    index = updateHistory_.back().index();

                    symmetricLatinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                counter = 1;
                latinSquaresCounter += symmetricLatinSquare.checkDiagonal();

                index = updateHistory_.back().index();

                symmetricLatinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        // std::cout << "Iterations: " << iterations << std::endl;

        return latinSquaresCounter;
    }

    const boost::multiprecision::mpz_int Generator::symmetricCount(
        SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        if (!symmetricLatinSquare.notFilled()) {
            return 1;
        }

        uint_fast16_t index = DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        EntropyData entropyData;

        updateHistory_.reserve(symmetricLatinSquare.notFilled());
        backtrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (updateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        symmetricLatinSquare.set(
                            backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    index = updateHistory_.back().index();

                    symmetricLatinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                counter = 1;
                latinSquaresCounter += symmetricLatinSquare.checkDiagonal();

                index = updateHistory_.back().index();

                symmetricLatinSquare.clearAndRemove(index, updateHistory_.back().entropyData());
                symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        return latinSquaresCounter;
    }
}
