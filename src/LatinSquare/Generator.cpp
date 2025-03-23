#include "Generator.hpp"

#include <cpp/random.hpp>

#include "Cell.hpp"
#include "EntropyData.hpp"




#include "Utils.hpp"

namespace LatinSquare {
    const LatinSquare Generator::random(const uint_fast8_t size, const Type type) noexcept {
        cpp::splitmix64 splitmix64;
        LatinSquare latinSquare(size, type, splitmix64);

        uint_fast8_t number;
        EntropyData entropyData;

        uint_fast16_t gridSize = size;
        gridSize *= size;
        updateHistory_.reserve(gridSize);
        backtrackingHistory_.reserve(gridSize);

        uint_fast16_t counter = 0;
        uint_fast32_t iterations = 0;

        while (latinSquare.notFilled()) {
            auto& cell = latinSquare.randomMinEntropyCell();

            if (cell.entropy()) {
                counter = 0;

                number = cell.numbers()[splitmix64.next() % cell.numbers().size()];
                entropyData = cell.entropyData();
                latinSquare.fillAndClear(cell, number);

                updateHistory_.emplace_back(cell.index(), number, entropyData, latinSquare.update(cell, number));

                if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                    backtrackingHistory_.emplace_back(cell.index(), entropyData);
                }
            } else {
                if (++counter > 1) {
                    latinSquare.set(backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                    backtrackingHistory_.pop_back();

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                latinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (++iterations > MAX_ITERATIONS) {
                latinSquare.set(type);
                iterations = 0;
            }
        }

        return latinSquare;
    }

    const LatinSquare Generator::random(const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers) noexcept {
        cpp::splitmix64 splitmix64;
        LatinSquare latinSquare(size, numbers, splitmix64);

        uint_fast8_t number;
        EntropyData entropyData;

        uint_fast16_t gridSize = size;
        gridSize *= size;
        updateHistory_.reserve(gridSize);
        backtrackingHistory_.reserve(gridSize);

        uint_fast16_t counter = 0;
        uint_fast32_t iterations = 0;

        while (latinSquare.notFilled()) {
            auto& cell = latinSquare.randomMinEntropyCell();

            if (cell.entropy()) {
                counter = 0;

                number = cell.numbers()[splitmix64.next() % cell.numbers().size()];
                entropyData = cell.entropyData();
                latinSquare.fillAndClear(cell, number);

                updateHistory_.emplace_back(cell.index(), number, entropyData, latinSquare.update(cell, number));

                if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                    backtrackingHistory_.emplace_back(cell.index(), entropyData);
                }
            } else {
                if (++counter > 1) {
                    latinSquare.set(backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                    backtrackingHistory_.pop_back();

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                latinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (++iterations > MAX_ITERATIONS) {
                latinSquare.set(numbers);
                iterations = 0;
            }
        }

        return latinSquare;
    }

    const boost::multiprecision::mpz_int Generator::count(const uint_fast8_t size, const Type type) noexcept {
        LatinSquare latinSquare(size, type);

        if (latinSquare.notFilled() < 2) {
            return 1;
        }

        uint_fast8_t number;
        EntropyData entropyData;

        uint_fast16_t gridSize = size;
        gridSize *= size;
        updateHistory_.reserve(gridSize);
        backtrackingHistory_.reserve(gridSize);

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled() > 1) {
                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(cell.index(), number, entropyData, latinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        latinSquare.set(backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    latinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                    latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                counter = 1;
                latinSquaresCounter += latinSquare.minEntropyCell().entropy();

                latinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        return latinSquaresCounter;
    }

    const boost::multiprecision::mpz_int Generator::count(LatinSquare& latinSquare) noexcept {
        if (latinSquare.notFilled() < 2) {
            return 1;
        }

        uint_fast8_t number;
        EntropyData entropyData;

        uint_fast16_t gridSize = latinSquare.size();
        gridSize *= latinSquare.size();
        updateHistory_.reserve(gridSize);
        backtrackingHistory_.reserve(gridSize);

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled() > 1) {
                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(cell.index(), number, entropyData, latinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        latinSquare.set(backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    latinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                    latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                counter = 1;
                latinSquaresCounter += latinSquare.minEntropyCell().entropy();

                latinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        return latinSquaresCounter;
    }

    const SymmetricLatinSquare Generator::symmetricRandom(const uint_fast8_t size, const Type type) noexcept {
        cpp::splitmix64 splitmix64;
        SymmetricLatinSquare symmetricLatinSquare(size, type, splitmix64);

        uint_fast8_t number;
        EntropyData entropyData;

        uint_fast16_t entropyTriangularGridSize = size;
        entropyTriangularGridSize *= (size - 1);
        entropyTriangularGridSize >>= 1;
        updateHistory_.reserve(entropyTriangularGridSize);
        backtrackingHistory_.reserve(entropyTriangularGridSize);

        uint_fast16_t counter = 0;
        uint_fast32_t iterations = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.randomMinEntropyCell();

                if (cell.entropy()) {
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
                    if (++counter > 1) {
                        symmetricLatinSquare.set(
                            backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.clearAndRemove(
                        updateHistory_.back().index(), updateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else if (symmetricLatinSquare.checkDiagonal()) {
                symmetricLatinSquare.fillGrid();
                break;
            } else {
                counter = 1;

                symmetricLatinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (++iterations > SYMMETRIC_MAX_ITERATIONS) {
                symmetricLatinSquare.set(type);
                iterations = 0;
            }
        }

        return symmetricLatinSquare;
    }

    const SymmetricLatinSquare Generator::symmetricRandom(
        const uint_fast8_t size, const std::vector<uint_fast8_t>& numbers) noexcept {
        cpp::splitmix64 splitmix64;
        SymmetricLatinSquare symmetricLatinSquare(size, numbers, splitmix64);

        uint_fast8_t number;
        EntropyData entropyData;

        uint_fast16_t entropyTriangularGridSize = size;
        entropyTriangularGridSize *= (size - 1);
        entropyTriangularGridSize >>= 1;
        updateHistory_.reserve(entropyTriangularGridSize);
        backtrackingHistory_.reserve(entropyTriangularGridSize);

        uint_fast16_t counter = 0;
        uint_fast32_t iterations = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.randomMinEntropyCell();

                if (cell.entropy()) {
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
                    if (++counter > 1) {
                        symmetricLatinSquare.set(
                            backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.clearAndRemove(
                        updateHistory_.back().index(), updateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else if (symmetricLatinSquare.checkDiagonal()) {
                symmetricLatinSquare.fillGrid();
                break;
            } else {
                counter = 1;

                symmetricLatinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (++iterations > SYMMETRIC_MAX_ITERATIONS) {
                symmetricLatinSquare.set(numbers);
                iterations = 0;
            }
        }

        return symmetricLatinSquare;
    }

    const boost::multiprecision::mpz_int Generator::symmetricCount(const uint_fast8_t size, const Type type) noexcept {
        SymmetricLatinSquare symmetricLatinSquare(size, type);

        if (!symmetricLatinSquare.notFilled()) {
            return 1;
        }

        uint_fast8_t number;
        EntropyData entropyData;

        uint_fast16_t entropyTriangularGridSize = size;
        entropyTriangularGridSize *= (size - 1);
        entropyTriangularGridSize >>= 1;
        updateHistory_.reserve(entropyTriangularGridSize);
        backtrackingHistory_.reserve(entropyTriangularGridSize);

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        symmetricLatinSquare.set(
                            backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.clearAndRemove(
                        updateHistory_.back().index(), updateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                counter = 1;
                latinSquaresCounter += symmetricLatinSquare.checkDiagonal();

                symmetricLatinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        return latinSquaresCounter;
    }

    // TODO: this code does not work so find problem here
    const boost::multiprecision::mpz_int Generator::symmetricCount(
        SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        if (!symmetricLatinSquare.notFilled()) {
            return 1;
        }

        uint_fast8_t number;
        EntropyData entropyData;

        uint_fast16_t entropyTriangularGridSize = symmetricLatinSquare.size();
        entropyTriangularGridSize *= (symmetricLatinSquare.size() - 1);
        entropyTriangularGridSize >>= 1;
        updateHistory_.reserve(entropyTriangularGridSize);
        backtrackingHistory_.reserve(entropyTriangularGridSize);

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    updateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (backtrackingHistory_.empty() || cell.index() != backtrackingHistory_.back().index()) {
                        backtrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        symmetricLatinSquare.set(
                            backtrackingHistory_.back().index(), backtrackingHistory_.back().entropyData());

                        backtrackingHistory_.pop_back();

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.clearAndRemove(
                        updateHistory_.back().index(), updateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                counter = 1;
                latinSquaresCounter += symmetricLatinSquare.checkDiagonal();

                symmetricLatinSquare.clearAndRemove(updateHistory_.back().index(), updateHistory_.back().entropyData());
                symmetricLatinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        return latinSquaresCounter;
    }
}
