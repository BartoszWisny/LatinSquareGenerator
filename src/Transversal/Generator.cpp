#include "Generator.hpp"

#include <cpp/random.hpp>

#include "Constants.hpp"
#include "LatinSquare/EntropyData.hpp"
#include "LatinSquare/Region.hpp"
#include "SymmetricCellUpdateData.hpp"

namespace Transversal {
    const std::vector<uint_fast16_t> Generator::random(LatinSquare::LatinSquare& latinSquare) noexcept {
        cpp::splitmix64 splitmix64;
        std::vector<uint_fast16_t> transversal;
        transversal.reserve(latinSquare.size());

        uint_fast16_t cellIndex;
        uint_fast8_t regionIndex;

        updateHistory_.reserve(latinSquare.size());
        backtrackingHistory_.reserve(latinSquare.size());

        uint_fast8_t counter = 0;
        uint_fast32_t iterations = 0;

        while (transversal.size() < latinSquare.size()) {
            ++iterations;

            auto& region = latinSquare.randomMinEntropyRegion();

            if (region.entropy()) {
                counter = 0;

                const auto& cellIndexes = region.enabledCellIndexes();
                cellIndex = cellIndexes[splitmix64.next() % cellIndexes.size()];
                transversal.emplace_back(cellIndex);
                latinSquare.disable(cellIndex);

                updateHistory_.emplace_back(cellIndex, latinSquare.disableAndDecrease(cellIndex));
                backtrackingHistory_.emplace_back(region.index(), cellIndex);
            } else {
                if (++counter > 1) {
                    latinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                    regionIndex = backtrackingHistory_.back().regionIndex();

                    backtrackingHistory_.pop_back();

                    while (backtrackingHistory_.size() && regionIndex == backtrackingHistory_.back().regionIndex()) {
                        latinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                        backtrackingHistory_.pop_back();
                    }

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
        uint_fast8_t transversalSize = 0;
        uint_fast8_t almostSize = latinSquare.size();
        --almostSize;

        if (!almostSize) {
            return 1;
        }

        uint_fast16_t cellIndex;
        uint_fast8_t regionIndex;

        updateHistory_.reserve(latinSquare.size());
        backtrackingHistory_.reserve(latinSquare.size());

        boost::multiprecision::mpz_int transversalsCounter = 0;
        uint_fast8_t counter = 0;

        while (true) {
            if (transversalSize < almostSize) {
                auto& region = latinSquare.minEntropyRegion();

                if (region.entropy()) {
                    ++transversalSize;
                    counter = 0;

                    cellIndex = region.enabledCellIndexes()[0];
                    latinSquare.disable(cellIndex);

                    updateHistory_.emplace_back(cellIndex, latinSquare.disableAndDecrease(cellIndex));
                    backtrackingHistory_.emplace_back(region.index(), cellIndex);
                } else {
                    --transversalSize;

                    if (++counter > 1) {
                        latinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                        regionIndex = backtrackingHistory_.back().regionIndex();

                        backtrackingHistory_.pop_back();

                        while (backtrackingHistory_.size()
                               && regionIndex == backtrackingHistory_.back().regionIndex()) {
                            latinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                            backtrackingHistory_.pop_back();
                        }

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    latinSquare.enable(updateHistory_.back().index());
                    latinSquare.enableAndIncrease(updateHistory_.back().indexes());

                    updateHistory_.pop_back();
                }
            } else {
                --transversalSize;
                counter = 1;
                transversalsCounter += latinSquare.minEntropyRegion().entropy();

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

    const std::vector<MinMaxData>& Generator::minMax(const uint_fast8_t size, const LatinSquare::Type type) noexcept {
        LatinSquare::LatinSquare latinSquare(size, type);

        boost::multiprecision::mpz_int transversalsCounter;
        latinSquaresCounters_.reserve(2);

        if (latinSquare.notFilled() < 2) {
            if (latinSquare.notFilled()) {
                auto& cell = latinSquare.minEntropyCell();
                latinSquare.fillAndClear(cell, cell.numbers()[0]);
            }

            latinSquare.setRegions();
            transversalsCounter = count(latinSquare);
            latinSquaresCounters_.emplace_back(transversalsCounter, 1, latinSquare);
            latinSquaresCounters_.emplace_back(transversalsCounter, 1, latinSquare);

            return latinSquaresCounters_;
        }

        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(latinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(latinSquare.notFilled());

        latinSquaresCounters_.emplace_back(factorial(size), 1, latinSquare);
        latinSquaresCounters_.emplace_back(-1, 1, latinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled() > 1) {
                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

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

                    latinSquare.clearAndRemove(
                        latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                    latinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                counter = 1;

                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

                    latinSquare.setRegions();
                    transversalsCounter = count(latinSquare);

                    if (transversalsCounter < latinSquaresCounters_[0].counter()) {
                        latinSquaresCounters_[0].set(transversalsCounter);
                        latinSquaresCounters_[0].reset();
                        latinSquaresCounters_[0].set(latinSquare);
                    } else if (transversalsCounter == latinSquaresCounters_[0].counter()) {
                        latinSquaresCounters_[0].increase();
                    }

                    if (transversalsCounter > latinSquaresCounters_[1].counter()) {
                        latinSquaresCounters_[1].set(transversalsCounter);
                        latinSquaresCounters_[1].reset();
                        latinSquaresCounters_[1].set(latinSquare);
                    } else if (transversalsCounter == latinSquaresCounters_[1].counter()) {
                        latinSquaresCounters_[1].increase();
                    }

                    latinSquare.clear(cell.index(), entropyData);
                }

                latinSquare.clearAndRemove(
                    latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                latinSquare.restore(
                    latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                latinSquareUpdateHistory_.pop_back();
            }
        }

        if (latinSquaresCounters_[0].counter() > latinSquaresCounters_[1].counter()) {
            latinSquaresCounters_[0].set(0);
            latinSquaresCounters_[1].set(0);
        }

        return latinSquaresCounters_;
    }

    const std::vector<MinMaxData>& Generator::minMax(LatinSquare::LatinSquare& latinSquare) noexcept {
        boost::multiprecision::mpz_int transversalsCounter;
        latinSquaresCounters_.reserve(2);

        if (latinSquare.notFilled() < 2) {
            if (latinSquare.notFilled()) {
                auto& cell = latinSquare.minEntropyCell();
                latinSquare.fillAndClear(cell, cell.numbers()[0]);
            }

            latinSquare.setRegions();
            transversalsCounter = count(latinSquare);
            latinSquaresCounters_.emplace_back(transversalsCounter, 1, latinSquare);
            latinSquaresCounters_.emplace_back(transversalsCounter, 1, latinSquare);

            return latinSquaresCounters_;
        }

        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(latinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(latinSquare.notFilled());

        latinSquaresCounters_.emplace_back(factorial(latinSquare.size()), 1, latinSquare);
        latinSquaresCounters_.emplace_back(-1, 1, latinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled() > 1) {
                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

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

                    latinSquare.clearAndRemove(
                        latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                    latinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                counter = 1;

                auto& cell = latinSquare.minEntropyCell();

                if (cell.entropy()) {
                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

                    latinSquare.setRegions();
                    transversalsCounter = count(latinSquare);

                    if (transversalsCounter < latinSquaresCounters_[0].counter()) {
                        latinSquaresCounters_[0].set(transversalsCounter);
                        latinSquaresCounters_[0].reset();
                        latinSquaresCounters_[0].set(latinSquare);
                    } else if (transversalsCounter == latinSquaresCounters_[0].counter()) {
                        latinSquaresCounters_[0].increase();
                    }

                    if (transversalsCounter > latinSquaresCounters_[1].counter()) {
                        latinSquaresCounters_[1].set(transversalsCounter);
                        latinSquaresCounters_[1].reset();
                        latinSquaresCounters_[1].set(latinSquare);
                    } else if (transversalsCounter == latinSquaresCounters_[1].counter()) {
                        latinSquaresCounters_[1].increase();
                    }

                    latinSquare.clear(cell.index(), entropyData);
                }

                latinSquare.clearAndRemove(
                    latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                latinSquare.restore(
                    latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                latinSquareUpdateHistory_.pop_back();
            }
        }

        if (latinSquaresCounters_[0].counter() > latinSquaresCounters_[1].counter()) {
            latinSquaresCounters_[0].set(0);
            latinSquaresCounters_[1].set(0);
        }

        return latinSquaresCounters_;
    }

    const std::vector<uint_fast16_t> Generator::symmetricRandom(
        LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        cpp::splitmix64 splitmix64;
        std::vector<uint_fast16_t> transversal;
        transversal.reserve(symmetricLatinSquare.size());

        uint_fast16_t cellIndex;
        uint_fast8_t regionIndex;

        updateHistory_.reserve(symmetricLatinSquare.size());
        backtrackingHistory_.reserve(symmetricLatinSquare.size());

        uint_fast8_t counter = 0;
        uint_fast32_t iterations = 0;

        while (transversal.size() < symmetricLatinSquare.size()) {
            ++iterations;

            auto& region = symmetricLatinSquare.randomMinEntropyRegion();

            if (region.entropy()) {
                counter = 0;

                const auto& cellIndexes = region.enabledCellIndexes();
                cellIndex = cellIndexes[splitmix64.next() % cellIndexes.size()];
                transversal.emplace_back(cellIndex);
                symmetricLatinSquare.disable(cellIndex);

                updateHistory_.emplace_back(cellIndex, symmetricLatinSquare.disableAndDecrease(cellIndex));
                backtrackingHistory_.emplace_back(region.index(), cellIndex);
            } else {
                if (++counter > 1) {
                    symmetricLatinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                    regionIndex = backtrackingHistory_.back().regionIndex();

                    backtrackingHistory_.pop_back();

                    while (backtrackingHistory_.size() && regionIndex == backtrackingHistory_.back().regionIndex()) {
                        symmetricLatinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                        backtrackingHistory_.pop_back();
                    }

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                transversal.pop_back();

                symmetricLatinSquare.enable(updateHistory_.back().index());
                symmetricLatinSquare.enableAndIncrease(updateHistory_.back().indexes());

                updateHistory_.pop_back();
            }

            if (iterations > MAX_ITERATIONS) {
                symmetricLatinSquare.setRegions();
                transversal.clear();
                iterations = 0;
            }
        }

        return transversal;
    }

    const boost::multiprecision::mpz_int Generator::symmetricCount(
        LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        uint_fast8_t transversalSize = 0;
        uint_fast8_t almostSize = symmetricLatinSquare.size();
        --almostSize;

        if (!almostSize) {
            return 1;
        }

        uint_fast16_t cellIndex;
        uint_fast8_t regionIndex;

        updateHistory_.reserve(symmetricLatinSquare.size());
        backtrackingHistory_.reserve(symmetricLatinSquare.size());

        boost::multiprecision::mpz_int transversalsCounter = 0;
        uint_fast8_t counter = 0;

        while (true) {
            if (transversalSize < almostSize) {
                auto& region = symmetricLatinSquare.minEntropyRegion();

                if (region.entropy()) {
                    ++transversalSize;
                    counter = 0;

                    cellIndex = region.enabledCellIndexes()[0];
                    symmetricLatinSquare.disable(cellIndex);

                    updateHistory_.emplace_back(cellIndex, symmetricLatinSquare.disableAndDecrease(cellIndex));
                    backtrackingHistory_.emplace_back(region.index(), cellIndex);
                } else {
                    --transversalSize;

                    if (++counter > 1) {
                        symmetricLatinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                        regionIndex = backtrackingHistory_.back().regionIndex();

                        backtrackingHistory_.pop_back();

                        while (backtrackingHistory_.size()
                               && regionIndex == backtrackingHistory_.back().regionIndex()) {
                            symmetricLatinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                            backtrackingHistory_.pop_back();
                        }

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.enable(updateHistory_.back().index());
                    symmetricLatinSquare.enableAndIncrease(updateHistory_.back().indexes());

                    updateHistory_.pop_back();
                }
            } else {
                --transversalSize;
                counter = 1;
                transversalsCounter += symmetricLatinSquare.minEntropyRegion().entropy();

                symmetricLatinSquare.enable(updateHistory_.back().index());
                symmetricLatinSquare.enableAndIncrease(updateHistory_.back().indexes());

                updateHistory_.pop_back();
            }
        }

        return transversalsCounter;
    }

    const std::vector<SymmetricMinMaxData>& Generator::symmetricMinMax(
        const uint_fast8_t size, const LatinSquare::Type type) noexcept {
        LatinSquare::SymmetricLatinSquare symmetricLatinSquare(size, type);

        boost::multiprecision::mpz_int transversalsCounter;
        symmetricLatinSquaresCounters_.reserve(2);

        if (!symmetricLatinSquare.notFilled()) {
            if (symmetricLatinSquare.checkDiagonal()) {
                symmetricLatinSquare.fillGrid();

                symmetricLatinSquare.setRegions();
                transversalsCounter = symmetricCount(symmetricLatinSquare);
                symmetricLatinSquaresCounters_.emplace_back(transversalsCounter, 1, symmetricLatinSquare);
                symmetricLatinSquaresCounters_.emplace_back(transversalsCounter, 1, symmetricLatinSquare);

                return symmetricLatinSquaresCounters_;
            }

            symmetricLatinSquaresCounters_.emplace_back(0, 0, symmetricLatinSquare);
            symmetricLatinSquaresCounters_.emplace_back(0, 0, symmetricLatinSquare);

            return symmetricLatinSquaresCounters_;
        }

        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(symmetricLatinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        symmetricLatinSquaresCounters_.emplace_back(factorial(size), 1, symmetricLatinSquare);
        symmetricLatinSquaresCounters_.emplace_back(-1, 1, symmetricLatinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        symmetricLatinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                            latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.clearAndRemove(
                        latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                counter = 1;

                if (symmetricLatinSquare.checkDiagonal()) {
                    symmetricLatinSquare.fillGrid();

                    symmetricLatinSquare.setRegions();
                    transversalsCounter = symmetricCount(symmetricLatinSquare);

                    if (transversalsCounter < symmetricLatinSquaresCounters_[0].counter()) {
                        symmetricLatinSquaresCounters_[0].set(transversalsCounter);
                        symmetricLatinSquaresCounters_[0].reset();
                        symmetricLatinSquaresCounters_[0].set(symmetricLatinSquare);
                    } else if (transversalsCounter == symmetricLatinSquaresCounters_[0].counter()) {
                        symmetricLatinSquaresCounters_[0].increase();
                    }

                    if (transversalsCounter > symmetricLatinSquaresCounters_[1].counter()) {
                        symmetricLatinSquaresCounters_[1].set(transversalsCounter);
                        symmetricLatinSquaresCounters_[1].reset();
                        symmetricLatinSquaresCounters_[1].set(symmetricLatinSquare);
                    } else if (transversalsCounter == symmetricLatinSquaresCounters_[1].counter()) {
                        symmetricLatinSquaresCounters_[1].increase();
                    }

                    symmetricLatinSquare.clearDiagonal();
                }

                if (latinSquareUpdateHistory_.empty()) {
                    break;
                }

                symmetricLatinSquare.clearAndRemove(
                    latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                symmetricLatinSquare.restore(
                    latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                latinSquareUpdateHistory_.pop_back();
            }
        }

        if (symmetricLatinSquaresCounters_[0].counter() > symmetricLatinSquaresCounters_[1].counter()) {
            symmetricLatinSquaresCounters_[0].set(0);
            symmetricLatinSquaresCounters_[1].set(0);
        }

        return symmetricLatinSquaresCounters_;
    }

    const std::vector<SymmetricMinMaxData>& Generator::symmetricMinMax(
        LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        boost::multiprecision::mpz_int transversalsCounter;
        symmetricLatinSquaresCounters_.reserve(2);

        if (!symmetricLatinSquare.notFilled()) {
            if (symmetricLatinSquare.checkDiagonal()) {
                symmetricLatinSquare.fillGrid();

                symmetricLatinSquare.setRegions();
                transversalsCounter = symmetricCount(symmetricLatinSquare);
                symmetricLatinSquaresCounters_.emplace_back(transversalsCounter, 1, symmetricLatinSquare);
                symmetricLatinSquaresCounters_.emplace_back(transversalsCounter, 1, symmetricLatinSquare);

                return symmetricLatinSquaresCounters_;
            }

            symmetricLatinSquaresCounters_.emplace_back(0, 0, symmetricLatinSquare);
            symmetricLatinSquaresCounters_.emplace_back(0, 0, symmetricLatinSquare);

            return symmetricLatinSquaresCounters_;
        }

        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(symmetricLatinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        symmetricLatinSquaresCounters_.emplace_back(factorial(symmetricLatinSquare.size()), 1, symmetricLatinSquare);
        symmetricLatinSquaresCounters_.emplace_back(-1, 1, symmetricLatinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        symmetricLatinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                            latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.clearAndRemove(
                        latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                counter = 1;

                if (symmetricLatinSquare.checkDiagonal()) {
                    symmetricLatinSquare.fillGrid();

                    symmetricLatinSquare.setRegions();
                    transversalsCounter = symmetricCount(symmetricLatinSquare);

                    if (transversalsCounter < symmetricLatinSquaresCounters_[0].counter()) {
                        symmetricLatinSquaresCounters_[0].set(transversalsCounter);
                        symmetricLatinSquaresCounters_[0].reset();
                        symmetricLatinSquaresCounters_[0].set(symmetricLatinSquare);
                    } else if (transversalsCounter == symmetricLatinSquaresCounters_[0].counter()) {
                        symmetricLatinSquaresCounters_[0].increase();
                    }

                    if (transversalsCounter > symmetricLatinSquaresCounters_[1].counter()) {
                        symmetricLatinSquaresCounters_[1].set(transversalsCounter);
                        symmetricLatinSquaresCounters_[1].reset();
                        symmetricLatinSquaresCounters_[1].set(symmetricLatinSquare);
                    } else if (transversalsCounter == symmetricLatinSquaresCounters_[1].counter()) {
                        symmetricLatinSquaresCounters_[1].increase();
                    }

                    symmetricLatinSquare.clearDiagonal();
                }

                if (latinSquareUpdateHistory_.empty()) {
                    break;
                }

                symmetricLatinSquare.clearAndRemove(
                    latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                symmetricLatinSquare.restore(
                    latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                latinSquareUpdateHistory_.pop_back();
            }
        }

        if (symmetricLatinSquaresCounters_[0].counter() > symmetricLatinSquaresCounters_[1].counter()) {
            symmetricLatinSquaresCounters_[0].set(0);
            symmetricLatinSquaresCounters_[1].set(0);
        }

        return symmetricLatinSquaresCounters_;
    }

    // const std::vector<uint_fast16_t> Generator::symmetricTriangularRandom(
    //     LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept {
    //     cpp::splitmix64 splitmix64;
    //     std::vector<uint_fast16_t> transversal;
    //     transversal.reserve(symmetricLatinSquare.size());

    //     uint_fast16_t cellIndex;
    //     uint_fast8_t regionIndex;

    //     symmetricUpdateHistory_.reserve(symmetricLatinSquare.size());
    //     symmetricBacktrackingHistory_.reserve(symmetricLatinSquare.size());

    //     uint_fast8_t counter = 0;
    //     uint_fast32_t iterations = 0;

    //     while (transversal.size() < symmetricLatinSquare.size()) {
    //         ++iterations;

    //         auto& region = symmetricLatinSquare.randomMinEntropyTriangularRegion();

    //         if (region.entropy()) {
    //             counter = 0;

    //             const auto& cellIndexes = region.triangularLocalEnabledCellIndexes();
    //             cellIndex = cellIndexes[splitmix64.next() % cellIndexes.size()];
    //             transversal.emplace_back(cellIndex);
    //             const auto& symmetricCellUpdateData = symmetricLatinSquare.triangularDisable(cellIndex, region.index());

    //             symmetricUpdateHistory_.emplace_back(region.index(), symmetricCellUpdateData,
    //                 symmetricLatinSquare.triangularDisableAndDecrease(cellIndex, region.index()));
    //             symmetricBacktrackingHistory_.emplace_back(region.index(), symmetricCellUpdateData);
    //         } else {
    //             if (++counter > 1) {
    //                 regionIndex = symmetricBacktrackingHistory_.back().regionIndex();

    //                 symmetricLatinSquare.triangularEnableAndIncrease(
    //                     symmetricBacktrackingHistory_.back().cellUpdateData()[0]);

    //                 symmetricBacktrackingHistory_.pop_back();

    //                 while (symmetricBacktrackingHistory_.size()
    //                        && regionIndex == symmetricBacktrackingHistory_.back().regionIndex()) {
    //                     symmetricLatinSquare.triangularEnableAndIncrease(
    //                         symmetricBacktrackingHistory_.back().cellUpdateData()[0]);

    //                     symmetricBacktrackingHistory_.pop_back();
    //                 }

    //                 if (symmetricUpdateHistory_.empty()) {
    //                     break;
    //                 }
    //             }

    //             transversal.pop_back();

    //             symmetricLatinSquare.triangularEnable(
    //                 symmetricUpdateHistory_.back().regionIndex(), symmetricUpdateHistory_.back().cellUpdateData());
    //             symmetricLatinSquare.triangularEnableAndIncrease(symmetricUpdateHistory_.back().otherCellsUpdateData());

    //             symmetricUpdateHistory_.pop_back();
    //         }

    //         if (iterations > MAX_ITERATIONS) {
    //             symmetricLatinSquare.setNumberRegions();
    //             transversal.clear();
    //             iterations = 0;
    //         }
    //     }

    //     return transversal;
    // }

    const boost::multiprecision::mpz_int Generator::symmetricTriangularCount(
        LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        uint_fast8_t transversalSize = 0;
        uint_fast8_t almostSize = symmetricLatinSquare.size();
        --almostSize;

        if (!almostSize) {
            return 1;
        }

        uint_fast16_t cellIndex;
        uint_fast8_t regionIndex;

        symmetricUpdateHistory_.reserve(symmetricLatinSquare.size());
        symmetricBacktrackingHistory_.reserve(symmetricLatinSquare.size());

        boost::multiprecision::mpz_int transversalsCounter = 0;
        uint_fast8_t counter = 0;

        while (true) {
            if (transversalSize < almostSize) {
                auto& region = symmetricLatinSquare.minEntropyTriangularRegion();

                if (region.entropy()) {
                    ++transversalSize;
                    counter = 0;

                    cellIndex = region.triangularLocalEnabledCellIndexes()[0];
                    const auto& symmetricCellUpdateData =
                        symmetricLatinSquare.triangularDisable(cellIndex, region.index());

                    symmetricUpdateHistory_.emplace_back(region.index(), symmetricCellUpdateData,
                        symmetricLatinSquare.triangularDisableAndDecrease(cellIndex, region.index()));
                    symmetricBacktrackingHistory_.emplace_back(region.index(), symmetricCellUpdateData);
                } else {
                    --transversalSize;

                    if (++counter > 1) {
                        regionIndex = symmetricBacktrackingHistory_.back().regionIndex();

                        symmetricLatinSquare.triangularEnableAndIncrease(
                            symmetricBacktrackingHistory_.back().cellUpdateData()[0]);

                        symmetricBacktrackingHistory_.pop_back();

                        while (symmetricBacktrackingHistory_.size()
                               && regionIndex == symmetricBacktrackingHistory_.back().regionIndex()) {
                            symmetricLatinSquare.triangularEnableAndIncrease(
                                symmetricBacktrackingHistory_.back().cellUpdateData()[0]);

                            symmetricBacktrackingHistory_.pop_back();
                        }

                        if (symmetricUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.triangularEnable(
                        symmetricUpdateHistory_.back().regionIndex(), symmetricUpdateHistory_.back().cellUpdateData());
                    symmetricLatinSquare.triangularEnableAndIncrease(
                        symmetricUpdateHistory_.back().otherCellsUpdateData());

                    symmetricUpdateHistory_.pop_back();
                }
            } else {
                --transversalSize;
                counter = 1;
                transversalsCounter += symmetricLatinSquare.minEntropyTriangularRegion().entropy();

                symmetricLatinSquare.triangularEnable(
                    symmetricUpdateHistory_.back().regionIndex(), symmetricUpdateHistory_.back().cellUpdateData());
                symmetricLatinSquare.triangularEnableAndIncrease(symmetricUpdateHistory_.back().otherCellsUpdateData());

                symmetricUpdateHistory_.pop_back();
            }
        }

        return transversalsCounter;
    }

    const std::vector<SymmetricMinMaxData>& Generator::symmetricTriangularMinMax(
        const uint_fast8_t size, const LatinSquare::Type type) noexcept {
        LatinSquare::SymmetricLatinSquare symmetricLatinSquare(size, type);

        boost::multiprecision::mpz_int transversalsCounter;
        symmetricLatinSquaresCounters_.reserve(2);

        if (!symmetricLatinSquare.notFilled()) {
            if (symmetricLatinSquare.checkDiagonal()) {
                symmetricLatinSquare.fillDiagonal();

                symmetricLatinSquare.setNumberRegions();
                transversalsCounter = symmetricTriangularCount(symmetricLatinSquare);
                symmetricLatinSquaresCounters_.emplace_back(transversalsCounter, 1, symmetricLatinSquare);
                symmetricLatinSquaresCounters_.emplace_back(transversalsCounter, 1, symmetricLatinSquare);

                return symmetricLatinSquaresCounters_;
            }

            symmetricLatinSquaresCounters_.emplace_back(0, 0, symmetricLatinSquare);
            symmetricLatinSquaresCounters_.emplace_back(0, 0, symmetricLatinSquare);

            return symmetricLatinSquaresCounters_;
        }

        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(symmetricLatinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        symmetricLatinSquaresCounters_.emplace_back(factorial(size), 1, symmetricLatinSquare);
        symmetricLatinSquaresCounters_.emplace_back(-1, 1, symmetricLatinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        symmetricLatinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                            latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.clearAndRemove(
                        latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                counter = 1;

                if (symmetricLatinSquare.checkDiagonal()) {
                    symmetricLatinSquare.fillDiagonal();

                    symmetricLatinSquare.setNumberRegions();
                    transversalsCounter = symmetricTriangularCount(symmetricLatinSquare);

                    if (transversalsCounter < symmetricLatinSquaresCounters_[0].counter()) {
                        symmetricLatinSquaresCounters_[0].set(transversalsCounter);
                        symmetricLatinSquaresCounters_[0].reset();
                        symmetricLatinSquaresCounters_[0].set(symmetricLatinSquare);
                    } else if (transversalsCounter == symmetricLatinSquaresCounters_[0].counter()) {
                        symmetricLatinSquaresCounters_[0].increase();
                    }

                    if (transversalsCounter > symmetricLatinSquaresCounters_[1].counter()) {
                        symmetricLatinSquaresCounters_[1].set(transversalsCounter);
                        symmetricLatinSquaresCounters_[1].reset();
                        symmetricLatinSquaresCounters_[1].set(symmetricLatinSquare);
                    } else if (transversalsCounter == symmetricLatinSquaresCounters_[1].counter()) {
                        symmetricLatinSquaresCounters_[1].increase();
                    }

                    symmetricLatinSquare.clearDiagonal();
                }

                if (latinSquareUpdateHistory_.empty()) {
                    break;
                }

                symmetricLatinSquare.clearAndRemove(
                    latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                symmetricLatinSquare.restore(
                    latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                latinSquareUpdateHistory_.pop_back();
            }
        }

        if (symmetricLatinSquaresCounters_[0].counter() > symmetricLatinSquaresCounters_[1].counter()) {
            symmetricLatinSquaresCounters_[0].set(0);
            symmetricLatinSquaresCounters_[1].set(0);
        }

        return symmetricLatinSquaresCounters_;
    }

    const std::vector<SymmetricMinMaxData>& Generator::symmetricTriangularMinMax(
        LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept {
        boost::multiprecision::mpz_int transversalsCounter;
        symmetricLatinSquaresCounters_.reserve(2);

        if (!symmetricLatinSquare.notFilled()) {
            if (symmetricLatinSquare.checkDiagonal()) {
                symmetricLatinSquare.fillDiagonal();

                symmetricLatinSquare.setNumberRegions();
                transversalsCounter = symmetricTriangularCount(symmetricLatinSquare);
                symmetricLatinSquaresCounters_.emplace_back(transversalsCounter, 1, symmetricLatinSquare);
                symmetricLatinSquaresCounters_.emplace_back(transversalsCounter, 1, symmetricLatinSquare);

                return symmetricLatinSquaresCounters_;
            }

            symmetricLatinSquaresCounters_.emplace_back(0, 0, symmetricLatinSquare);
            symmetricLatinSquaresCounters_.emplace_back(0, 0, symmetricLatinSquare);

            return symmetricLatinSquaresCounters_;
        }

        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(symmetricLatinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        symmetricLatinSquaresCounters_.emplace_back(factorial(symmetricLatinSquare.size()), 1, symmetricLatinSquare);
        symmetricLatinSquaresCounters_.emplace_back(-1, 1, symmetricLatinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell();

                if (cell.entropy()) {
                    counter = 0;

                    number = cell.numbers()[0];
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        symmetricLatinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                            latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    symmetricLatinSquare.clearAndRemove(
                        latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                    symmetricLatinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                counter = 1;

                if (symmetricLatinSquare.checkDiagonal()) {
                    symmetricLatinSquare.fillDiagonal();

                    symmetricLatinSquare.setNumberRegions();
                    transversalsCounter = symmetricTriangularCount(symmetricLatinSquare);

                    if (transversalsCounter < symmetricLatinSquaresCounters_[0].counter()) {
                        symmetricLatinSquaresCounters_[0].set(transversalsCounter);
                        symmetricLatinSquaresCounters_[0].reset();
                        symmetricLatinSquaresCounters_[0].set(symmetricLatinSquare);
                    } else if (transversalsCounter == symmetricLatinSquaresCounters_[0].counter()) {
                        symmetricLatinSquaresCounters_[0].increase();
                    }

                    if (transversalsCounter > symmetricLatinSquaresCounters_[1].counter()) {
                        symmetricLatinSquaresCounters_[1].set(transversalsCounter);
                        symmetricLatinSquaresCounters_[1].reset();
                        symmetricLatinSquaresCounters_[1].set(symmetricLatinSquare);
                    } else if (transversalsCounter == symmetricLatinSquaresCounters_[1].counter()) {
                        symmetricLatinSquaresCounters_[1].increase();
                    }

                    symmetricLatinSquare.clearDiagonal();
                }

                if (latinSquareUpdateHistory_.empty()) {
                    break;
                }

                symmetricLatinSquare.clearAndRemove(
                    latinSquareUpdateHistory_.back().index(), latinSquareUpdateHistory_.back().entropyData());
                symmetricLatinSquare.restore(
                    latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                latinSquareUpdateHistory_.pop_back();
            }
        }

        if (symmetricLatinSquaresCounters_[0].counter() > symmetricLatinSquaresCounters_[1].counter()) {
            symmetricLatinSquaresCounters_[0].set(0);
            symmetricLatinSquaresCounters_[1].set(0);
        }

        return symmetricLatinSquaresCounters_;
    }
}
