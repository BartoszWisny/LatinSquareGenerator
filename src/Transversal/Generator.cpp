#include "Generator.hpp"

#include "Constants.hpp"
#include "LatinSquare/EntropyData.hpp"
#include "LatinSquare/Region.hpp"

#include <iostream>

namespace Transversal {
    const std::vector<uint_fast16_t> Generator::random(LatinSquare::LatinSquare& latinSquare) noexcept {
        cpp::splitmix64 splitmix64;
        std::vector<uint_fast16_t> transversal;
        transversal.reserve(latinSquare.size());

        std::vector<uint_fast16_t> cellIndexes;
        uint_fast16_t cellIndex;
        uint_fast8_t regionIndex;
        std::vector<uint_fast16_t> indexes;

        updateHistory_.reserve(latinSquare.size());
        backtrackingHistory_.reserve(latinSquare.size());

        uint_fast8_t counter = 0;
        uint_fast32_t iterations = 0;

        while (transversal.size() < latinSquare.size()) {
            ++iterations;

            auto& region = latinSquare.randomMinEntropyRegion();

            if (region.entropy()) {
                counter = 0;

                cellIndexes = region.enabledCellIndexes();
                cellIndex = cellIndexes[splitmix64.next() % cellIndexes.size()];
                transversal.emplace_back(cellIndex);
                latinSquare.disable(cellIndex);

                updateHistory_.emplace_back(cellIndex, latinSquare.disableAndDecrease(cellIndex));

                if (backtrackingHistory_.empty() || cellIndex != backtrackingHistory_.back().cellIndex()) {
                    backtrackingHistory_.emplace_back(region.index(), cellIndex);
                }
            } else {
                if (++counter > 1) {
                    indexes.clear();
                    indexes.reserve(latinSquare.size());
                    indexes.emplace_back(backtrackingHistory_.back().cellIndex());

                    regionIndex = backtrackingHistory_.back().regionIndex();

                    backtrackingHistory_.pop_back();

                    while (!backtrackingHistory_.empty() && regionIndex == backtrackingHistory_.back().regionIndex()) {
                        indexes.emplace_back(backtrackingHistory_.back().cellIndex());
                        regionIndex = backtrackingHistory_.back().regionIndex();

                        backtrackingHistory_.pop_back();
                    }

                    latinSquare.enableAndIncrease(indexes);

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                transversal.pop_back();

                latinSquare.enable(updateHistory_.back().index());
                latinSquare.enableAndIncrease(updateHistory_.back().indexes());

                updateHistory_.pop_back();
            }

            if (iterations > MAX_ITERATIONS) {
                latinSquare.setRegions();
                transversal.clear();
                iterations = 0;
            }
        }

        return transversal;
    }

    const boost::multiprecision::mpz_int Generator::count(LatinSquare::LatinSquare& latinSquare) noexcept {
        std::vector<uint_fast16_t> transversal;
        transversal.reserve(latinSquare.size());

        uint_fast16_t cellIndex;
        uint_fast8_t regionIndex;
        std::vector<uint_fast16_t> indexes;

        updateHistory_.reserve(latinSquare.size());
        backtrackingHistory_.reserve(latinSquare.size());

        boost::multiprecision::mpz_int transversalsCounter = 0;
        uint_fast8_t counter = 0;

        while (true) {
            if (transversal.size() < latinSquare.size()) {
                auto& region = latinSquare.minEntropyRegion();

                if (region.entropy()) {
                    counter = 0;

                    cellIndex = region.enabledCellIndexes()[0];
                    transversal.emplace_back(cellIndex);
                    latinSquare.disable(cellIndex);

                    updateHistory_.emplace_back(cellIndex, latinSquare.disableAndDecrease(cellIndex));

                    if (backtrackingHistory_.empty() || cellIndex != backtrackingHistory_.back().cellIndex()) {
                        backtrackingHistory_.emplace_back(region.index(), cellIndex);
                    }
                } else {
                    if (++counter > 1) {
                        indexes.clear();
                        indexes.reserve(latinSquare.size());
                        indexes.emplace_back(backtrackingHistory_.back().cellIndex());

                        regionIndex = backtrackingHistory_.back().regionIndex();

                        backtrackingHistory_.pop_back();

                        while (!backtrackingHistory_.empty()
                               && regionIndex == backtrackingHistory_.back().regionIndex()) {
                            indexes.emplace_back(backtrackingHistory_.back().cellIndex());
                            regionIndex = backtrackingHistory_.back().regionIndex();

                            backtrackingHistory_.pop_back();
                        }

                        latinSquare.enableAndIncrease(indexes);

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    transversal.pop_back();

                    latinSquare.enable(updateHistory_.back().index());
                    latinSquare.enableAndIncrease(updateHistory_.back().indexes());

                    updateHistory_.pop_back();
                }
            } else {
                ++transversalsCounter;
                ++counter;

                transversal.pop_back();

                latinSquare.enable(updateHistory_.back().index());
                latinSquare.enableAndIncrease(updateHistory_.back().indexes());

                updateHistory_.pop_back();
            }
        }

        return transversalsCounter;
    }

    constexpr boost::multiprecision::mpz_int Generator::factorial(const uint_fast8_t size) noexcept {
        boost::multiprecision::mpz_int result = 1;

        for (uint_fast8_t i = 2; i <= size; ++i) {
            result *= i;
        }

        return result;
    }

    const std::array<MinMaxData, 2> Generator::minMax(const uint_fast8_t size, const LatinSquare::Type type) noexcept {
        LatinSquare::LatinSquare latinSquare(size, type);

        if (!latinSquare.notFilled()) {
            return {MinMaxData(1, latinSquare), MinMaxData(1, latinSquare)};
        }

        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        uint_fast16_t gridSize = size;
        gridSize *= size;
        latinSquareUpdateHistory_.reserve(gridSize);
        latinSquareBacktrackingHistory_.reserve(gridSize);

        std::array<MinMaxData, 2> latinSquaresCounters =
            {MinMaxData(factorial(size), latinSquare), MinMaxData(-1, latinSquare)};
        boost::multiprecision::mpz_int transversalsCounter;
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled()) {
                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fill(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, latinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        latinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                                        latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    latinSquare.clear(
                        latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                    latinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                latinSquare.setRegions();
                transversalsCounter = count(latinSquare);

                if (transversalsCounter < latinSquaresCounters[0].counter()) {
                    latinSquaresCounters[0].set(transversalsCounter);
                    latinSquaresCounters[0].set(latinSquare);
                }

                if (transversalsCounter > latinSquaresCounters[1].counter()) {
                    latinSquaresCounters[1].set(transversalsCounter);
                    latinSquaresCounters[1].set(latinSquare);
                }

                ++counter;

                latinSquare.clear(
                    latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                latinSquare.restore(
                    latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                latinSquareUpdateHistory_.pop_back();
            }
        }

        if (latinSquaresCounters[0].counter() > latinSquaresCounters[1].counter()) {
            latinSquaresCounters[0].set(0);
            latinSquaresCounters[1].set(0);
        }

        return latinSquaresCounters;
    }

    const std::array<MinMaxData, 2> Generator::minMax(LatinSquare::LatinSquare& latinSquare) noexcept {
        if (!latinSquare.notFilled()) {
            return {MinMaxData(1, latinSquare), MinMaxData(1, latinSquare)};
        }

        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        uint_fast16_t gridSize = latinSquare.size();
        gridSize *= latinSquare.size();
        latinSquareUpdateHistory_.reserve(gridSize);
        latinSquareBacktrackingHistory_.reserve(gridSize);

        std::array<MinMaxData, 2> latinSquaresCounters =
            {MinMaxData(factorial(latinSquare.size()), latinSquare), MinMaxData(-1, latinSquare)};
        boost::multiprecision::mpz_int transversalsCounter;
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled()) {
                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fill(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, latinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        latinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                                        latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    latinSquare.clear(
                        latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                    latinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                latinSquare.setRegions();
                transversalsCounter = count(latinSquare);

                if (transversalsCounter < latinSquaresCounters[0].counter()) {
                    latinSquaresCounters[0].set(transversalsCounter);
                    latinSquaresCounters[0].set(latinSquare);
                }

                if (transversalsCounter > latinSquaresCounters[1].counter()) {
                    latinSquaresCounters[1].set(transversalsCounter);
                    latinSquaresCounters[1].set(latinSquare);
                }

                ++counter;

                latinSquare.clear(
                    latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                latinSquare.restore(
                    latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                latinSquareUpdateHistory_.pop_back();
            }
        }

        if (latinSquaresCounters[0].counter() > latinSquaresCounters[1].counter()) {
            latinSquaresCounters[0].set(0);
            latinSquaresCounters[1].set(0);
        }

        return latinSquaresCounters;
    }
}
