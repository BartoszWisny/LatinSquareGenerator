#include "Generator.hpp"

#include "Cell.hpp"
#include "EntropyData.hpp"

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
            ++iterations;

            auto& cell = latinSquare.randomMinEntropyCell();

            if (cell.entropy()) {
                counter = 0;

                number = cell.numbers()[splitmix64.next() % cell.numbers().size()];
                entropyData = cell.entropyData();
                latinSquare.fill(cell, number);

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

                latinSquare.clear(updateHistory_.back().index(), updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }

            if (iterations > MAX_ITERATIONS) {
                latinSquare.reset(type);
                iterations = 0;
            }
        }

        return latinSquare;
    }

    const boost::multiprecision::mpz_int Generator::count(const uint_fast8_t size, const Type type) noexcept {
        LatinSquare latinSquare(size, type);

        if (!latinSquare.notFilled()) {
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
            if (latinSquare.notFilled()) {
                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fill(cell, number);

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

                    latinSquare.clear(updateHistory_.back().index(), updateHistory_.back().entropyData());
                    latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                ++latinSquaresCounter;
                ++counter;

                latinSquare.clear(updateHistory_.back().index(), updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        return latinSquaresCounter;
    }

    const boost::multiprecision::mpz_int Generator::count(LatinSquare& latinSquare) noexcept {
        if (!latinSquare.notFilled()) {
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
            if (latinSquare.notFilled()) {
                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fill(cell, number);

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

                    latinSquare.clear(updateHistory_.back().index(), updateHistory_.back().entropyData());
                    latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                    updateHistory_.pop_back();
                }
            } else {
                ++latinSquaresCounter;
                ++counter;

                latinSquare.clear(updateHistory_.back().index(), updateHistory_.back().entropyData());
                latinSquare.restore(updateHistory_.back().indexes(), updateHistory_.back().number());

                updateHistory_.pop_back();
            }
        }

        return latinSquaresCounter;
    }
}
