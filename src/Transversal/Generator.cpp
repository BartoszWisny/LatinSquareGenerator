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
        uint_fast8_t regionIndex = LatinSquare::DEFAULT_REGION_INDEX;

        updateHistory_.reserve(latinSquare.size());
        backtrackingHistory_.reserve(latinSquare.size());

        uint_fast8_t counter = 0;
        uint_fast32_t iterations = 0;

        while (transversal.size() < latinSquare.size()) {
            ++iterations;

            auto& region = latinSquare.randomMinEntropyRegion(regionIndex);

            if (region.entropy()) {
                regionIndex = LatinSquare::DEFAULT_REGION_INDEX;
                counter = 0;

                const auto& cellIndexes = region.enabledCellIndexes();
                cellIndex = cellIndexes[splitmix64.next() % cellIndexes.size()];
                transversal.emplace_back(cellIndex);
                latinSquare.disable(cellIndex);

                updateHistory_.emplace_back(region.index(), cellIndex, latinSquare.disableAndDecrease(cellIndex));
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

                regionIndex = updateHistory_.back().regionIndex();

                latinSquare.enable(updateHistory_.back().cellIndex());
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
        transversalSize_ = 0;
        almostSize_ = latinSquare.size();
        --almostSize_;

        if (!almostSize_) {
            return 1;
        }

        regionIndex_ = LatinSquare::DEFAULT_REGION_INDEX;

        updateHistory_.reserve(latinSquare.size());
        backtrackingHistory_.reserve(latinSquare.size());

        transversalsCounter_ = 0;
        counter_ = 0;

        while (true) {
            if (transversalSize_ < almostSize_) {
                auto& region = latinSquare.minEntropyRegion(regionIndex_);

                if (region.entropy()) {
                    ++transversalSize_;
                    regionIndex_ = LatinSquare::DEFAULT_REGION_INDEX;
                    counter_ = 0;

                    cellIndex_ = region.firstEnabledCellIndex();
                    latinSquare.disable(cellIndex_);

                    updateHistory_.emplace_back(region.index(), cellIndex_, latinSquare.disableAndDecrease(cellIndex_));
                    backtrackingHistory_.emplace_back(region.index(), cellIndex_);
                } else {
                    --transversalSize_;

                    if (++counter_ > 1) {
                        latinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                        regionIndex_ = backtrackingHistory_.back().regionIndex();

                        backtrackingHistory_.pop_back();

                        while (backtrackingHistory_.size()
                               && regionIndex_ == backtrackingHistory_.back().regionIndex()) {
                            latinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                            backtrackingHistory_.pop_back();
                        }

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    regionIndex_ = updateHistory_.back().regionIndex();

                    latinSquare.enable(updateHistory_.back().cellIndex());
                    latinSquare.enableAndIncrease(updateHistory_.back().indexes());

                    updateHistory_.pop_back();
                }
            } else {
                --transversalSize_;
                counter_ = 1;
                transversalsCounter_ += latinSquare.lastNotChosenRegion().entropy();

                regionIndex_ = updateHistory_.back().regionIndex();

                latinSquare.enable(updateHistory_.back().cellIndex());
                latinSquare.enableAndIncrease(updateHistory_.back().indexes());

                updateHistory_.pop_back();
            }
        }

        return transversalsCounter_;
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
                auto& cell = latinSquare.lastNotFilledCell();

                if (!cell.positiveEntropy()) {
                    latinSquaresCounters_.emplace_back(0, 0, latinSquare);
                    latinSquaresCounters_.emplace_back(0, 0, latinSquare);

                    return latinSquaresCounters_;
                }

                latinSquare.fillAndClear(cell, cell.firstNumber());
            }

            latinSquare.setRegions();
            transversalsCounter = count(latinSquare);
            latinSquaresCounters_.emplace_back(transversalsCounter, 1, latinSquare);
            latinSquaresCounters_.emplace_back(transversalsCounter, 1, latinSquare);

            return latinSquaresCounters_;
        }

        uint_fast16_t index = LatinSquare::DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(latinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(latinSquare.notFilled());

        latinSquaresCounters_.emplace_back(factorial(size), 1, latinSquare);
        latinSquaresCounters_.emplace_back(-1, 1, latinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled() > 1) {
                auto& cell = latinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = LatinSquare::DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, latinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (latinSquareUpdateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        latinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                                        latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    index = latinSquareUpdateHistory_.back().index();

                    latinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
                    latinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                counter = 1;

                auto& cell = latinSquare.lastNotFilledCell();

                if (cell.positiveEntropy()) {
                    number = cell.firstNumber();
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

                index = latinSquareUpdateHistory_.back().index();

                latinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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
                auto& cell = latinSquare.lastNotFilledCell();

                if (!cell.positiveEntropy()) {
                    latinSquaresCounters_.emplace_back(0, 0, latinSquare);
                    latinSquaresCounters_.emplace_back(0, 0, latinSquare);

                    return latinSquaresCounters_;
                }

                latinSquare.fillAndClear(cell, cell.firstNumber());
            }

            latinSquare.setRegions();
            transversalsCounter = count(latinSquare);
            latinSquaresCounters_.emplace_back(transversalsCounter, 1, latinSquare);
            latinSquaresCounters_.emplace_back(transversalsCounter, 1, latinSquare);

            return latinSquaresCounters_;
        }

        uint_fast16_t index = LatinSquare::DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(latinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(latinSquare.notFilled());

        latinSquaresCounters_.emplace_back(factorial(latinSquare.size()), 1, latinSquare);
        latinSquaresCounters_.emplace_back(-1, 1, latinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled() > 1) {
                auto& cell = latinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = LatinSquare::DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    latinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, latinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (latinSquareUpdateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        latinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                                        latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    index = latinSquareUpdateHistory_.back().index();

                    latinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
                    latinSquare.restore(
                        latinSquareUpdateHistory_.back().indexes(), latinSquareUpdateHistory_.back().number());

                    latinSquareUpdateHistory_.pop_back();
                }
            } else {
                counter = 1;

                auto& cell = latinSquare.lastNotFilledCell();

                if (cell.positiveEntropy()) {
                    number = cell.firstNumber();
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

                index = latinSquareUpdateHistory_.back().index();

                latinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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
        uint_fast8_t regionIndex = LatinSquare::DEFAULT_REGION_INDEX;

        updateHistory_.reserve(symmetricLatinSquare.size());
        backtrackingHistory_.reserve(symmetricLatinSquare.size());

        uint_fast8_t counter = 0;
        uint_fast32_t iterations = 0;

        while (transversal.size() < symmetricLatinSquare.size()) {
            ++iterations;

            auto& region = symmetricLatinSquare.randomMinEntropyRegion(regionIndex);

            if (region.entropy()) {
                regionIndex = LatinSquare::DEFAULT_REGION_INDEX;
                counter = 0;

                const auto& cellIndexes = region.enabledCellIndexes();
                cellIndex = cellIndexes[splitmix64.next() % cellIndexes.size()];
                transversal.emplace_back(cellIndex);
                symmetricLatinSquare.disable(cellIndex);

                updateHistory_.emplace_back(
                    region.index(), cellIndex, symmetricLatinSquare.disableAndDecrease(cellIndex));
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

                regionIndex = updateHistory_.back().regionIndex();

                symmetricLatinSquare.enable(updateHistory_.back().cellIndex());
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
        transversalSize_ = 0;
        almostSize_ = symmetricLatinSquare.size();
        --almostSize_;

        if (!almostSize_) {
            return 1;
        }

        regionIndex_ = LatinSquare::DEFAULT_REGION_INDEX;

        updateHistory_.reserve(symmetricLatinSquare.size());
        backtrackingHistory_.reserve(symmetricLatinSquare.size());

        transversalsCounter_ = 0;
        counter_ = 0;

        while (true) {
            if (transversalSize_ < almostSize_) {
                auto& region = symmetricLatinSquare.minEntropyRegion(regionIndex_);

                if (region.entropy()) {
                    ++transversalSize_;
                    regionIndex_ = LatinSquare::DEFAULT_REGION_INDEX;
                    counter_ = 0;

                    cellIndex_ = region.firstEnabledCellIndex();
                    symmetricLatinSquare.disable(cellIndex_);

                    updateHistory_.emplace_back(
                        region.index(), cellIndex_, symmetricLatinSquare.disableAndDecrease(cellIndex_));
                    backtrackingHistory_.emplace_back(region.index(), cellIndex_);
                } else {
                    --transversalSize_;

                    if (++counter_ > 1) {
                        symmetricLatinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                        regionIndex_ = backtrackingHistory_.back().regionIndex();

                        backtrackingHistory_.pop_back();

                        while (backtrackingHistory_.size()
                               && regionIndex_ == backtrackingHistory_.back().regionIndex()) {
                            symmetricLatinSquare.enableAndIncrease(backtrackingHistory_.back().cellIndex());

                            backtrackingHistory_.pop_back();
                        }

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    regionIndex_ = updateHistory_.back().regionIndex();

                    symmetricLatinSquare.enable(updateHistory_.back().cellIndex());
                    symmetricLatinSquare.enableAndIncrease(updateHistory_.back().indexes());

                    updateHistory_.pop_back();
                }
            } else {
                --transversalSize_;
                counter_ = 1;
                transversalsCounter_ += symmetricLatinSquare.lastNotChosenRegion().entropy();

                regionIndex_ = updateHistory_.back().regionIndex();

                symmetricLatinSquare.enable(updateHistory_.back().cellIndex());
                symmetricLatinSquare.enableAndIncrease(updateHistory_.back().indexes());

                updateHistory_.pop_back();
            }
        }

        return transversalsCounter_;
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

        uint_fast16_t index = LatinSquare::DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(symmetricLatinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        symmetricLatinSquaresCounters_.emplace_back(factorial(size), 1, symmetricLatinSquare);
        symmetricLatinSquaresCounters_.emplace_back(-1, 1, symmetricLatinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = LatinSquare::DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (latinSquareUpdateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        symmetricLatinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                            latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    index = latinSquareUpdateHistory_.back().index();

                    symmetricLatinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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

                index = latinSquareUpdateHistory_.back().index();

                symmetricLatinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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

        uint_fast16_t index = LatinSquare::DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(symmetricLatinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        symmetricLatinSquaresCounters_.emplace_back(factorial(symmetricLatinSquare.size()), 1, symmetricLatinSquare);
        symmetricLatinSquaresCounters_.emplace_back(-1, 1, symmetricLatinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = LatinSquare::DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (latinSquareUpdateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        symmetricLatinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                            latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    index = latinSquareUpdateHistory_.back().index();

                    symmetricLatinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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

                index = latinSquareUpdateHistory_.back().index();

                symmetricLatinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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
    //     uint_fast8_t regionIndex = LatinSquare::DEFAULT_REGION_INDEX;

    //     symmetricUpdateHistory_.reserve(symmetricLatinSquare.size());
    //     symmetricBacktrackingHistory_.reserve(symmetricLatinSquare.size());

    //     uint_fast8_t counter = 0;
    //     uint_fast32_t iterations = 0;

    //     while (transversal.size() < symmetricLatinSquare.size()) {
    //         ++iterations;

    //         auto& region = symmetricLatinSquare.randomMinEntropyTriangularRegion(regionIndex);

    //         if (region.entropy()) {
    //             regionIndex = LatinSquare::DEFAULT_REGION_INDEX;
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

    //             regionIndex = symmetricUpdateHistory_.back().regionIndex();

    //             symmetricLatinSquare.triangularEnable(regionIndex, symmetricUpdateHistory_.back().cellUpdateData());
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
        transversalSize_ = 0;
        almostSize_ = symmetricLatinSquare.size();
        --almostSize_;

        if (!almostSize_) {
            return 1;
        }

        regionIndex_ = LatinSquare::DEFAULT_REGION_INDEX;

        symmetricUpdateHistory_.reserve(symmetricLatinSquare.size());
        symmetricBacktrackingHistory_.reserve(symmetricLatinSquare.size());

        transversalsCounter_ = 0;
        counter_ = 0;

        while (true) {
            if (transversalSize_ < almostSize_) {
                auto& region = symmetricLatinSquare.minEntropyTriangularRegion(regionIndex_);

                if (region.entropy()) {
                    ++transversalSize_;
                    regionIndex_ = LatinSquare::DEFAULT_REGION_INDEX;
                    counter_ = 0;

                    cellIndex_ = region.firstTriangularLocalEnabledCellIndex();
                    const auto& symmetricCellUpdateData =
                        symmetricLatinSquare.triangularDisable(cellIndex_, region.index());

                    symmetricUpdateHistory_.emplace_back(region.index(), symmetricCellUpdateData,
                        symmetricLatinSquare.triangularDisableAndDecrease(cellIndex_, region.index()));
                    symmetricBacktrackingHistory_.emplace_back(region.index(), symmetricCellUpdateData);
                } else {
                    --transversalSize_;

                    if (++counter_ > 1) {
                        regionIndex_ = symmetricBacktrackingHistory_.back().regionIndex();

                        symmetricLatinSquare.triangularEnableAndIncrease(
                            symmetricBacktrackingHistory_.back().cellUpdateData()[0]);

                        symmetricBacktrackingHistory_.pop_back();

                        while (symmetricBacktrackingHistory_.size()
                               && regionIndex_ == symmetricBacktrackingHistory_.back().regionIndex()) {
                            symmetricLatinSquare.triangularEnableAndIncrease(
                                symmetricBacktrackingHistory_.back().cellUpdateData()[0]);

                            symmetricBacktrackingHistory_.pop_back();
                        }

                        if (symmetricUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    regionIndex_ = symmetricUpdateHistory_.back().regionIndex();

                    symmetricLatinSquare.triangularEnable(
                        regionIndex_, symmetricUpdateHistory_.back().cellUpdateData());
                    symmetricLatinSquare.triangularEnableAndIncrease(
                        symmetricUpdateHistory_.back().otherCellsUpdateData());

                    symmetricUpdateHistory_.pop_back();
                }
            } else {
                --transversalSize_;
                counter_ = 1;
                transversalsCounter_ += symmetricLatinSquare.lastNotChosenTriangularRegion().entropy();

                regionIndex_ = symmetricUpdateHistory_.back().regionIndex();

                symmetricLatinSquare.triangularEnable(regionIndex_, symmetricUpdateHistory_.back().cellUpdateData());
                symmetricLatinSquare.triangularEnableAndIncrease(symmetricUpdateHistory_.back().otherCellsUpdateData());

                symmetricUpdateHistory_.pop_back();
            }
        }

        return transversalsCounter_;
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

        uint_fast16_t index = LatinSquare::DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(symmetricLatinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        symmetricLatinSquaresCounters_.emplace_back(factorial(size), 1, symmetricLatinSquare);
        symmetricLatinSquaresCounters_.emplace_back(-1, 1, symmetricLatinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = LatinSquare::DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (latinSquareUpdateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        symmetricLatinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                            latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    index = latinSquareUpdateHistory_.back().index();

                    symmetricLatinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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

                index = latinSquareUpdateHistory_.back().index();

                symmetricLatinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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

        uint_fast16_t index = LatinSquare::DEFAULT_CELL_INDEX;
        uint_fast8_t number;
        LatinSquare::EntropyData entropyData;

        latinSquareUpdateHistory_.reserve(symmetricLatinSquare.notFilled());
        latinSquareBacktrackingHistory_.reserve(symmetricLatinSquare.notFilled());

        symmetricLatinSquaresCounters_.emplace_back(factorial(symmetricLatinSquare.size()), 1, symmetricLatinSquare);
        symmetricLatinSquaresCounters_.emplace_back(-1, 1, symmetricLatinSquare);
        uint_fast16_t counter = 0;

        while (true) {
            if (symmetricLatinSquare.notFilled()) {
                auto& cell = symmetricLatinSquare.minEntropyCell(index);

                if (cell.positiveEntropy()) {
                    index = LatinSquare::DEFAULT_CELL_INDEX;
                    counter = 0;

                    number = cell.firstNumber();
                    entropyData = cell.entropyData();
                    symmetricLatinSquare.fillAndClear(cell, number);

                    latinSquareUpdateHistory_.emplace_back(
                        cell.index(), number, entropyData, symmetricLatinSquare.update(cell, number));

                    if (latinSquareBacktrackingHistory_.empty()
                        || cell.index() != latinSquareBacktrackingHistory_.back().index()) {
                        latinSquareBacktrackingHistory_.emplace_back(cell.index(), entropyData);
                    }
                } else {
                    if (latinSquareUpdateHistory_.empty()) {
                        break;
                    }

                    if (++counter > 1) {
                        symmetricLatinSquare.set(latinSquareBacktrackingHistory_.back().index(),
                            latinSquareBacktrackingHistory_.back().entropyData());

                        latinSquareBacktrackingHistory_.pop_back();

                        if (latinSquareUpdateHistory_.empty()) {
                            break;
                        }
                    }

                    index = latinSquareUpdateHistory_.back().index();

                    symmetricLatinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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

                index = latinSquareUpdateHistory_.back().index();

                symmetricLatinSquare.clearAndRemove(index, latinSquareUpdateHistory_.back().entropyData());
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
