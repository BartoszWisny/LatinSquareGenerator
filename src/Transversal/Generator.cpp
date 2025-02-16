#include "Generator.hpp"

namespace Transversal {
    const std::vector<uint_fast16_t> Generator::random(LatinSquare::LatinSquare& latinSquare) {
        cpp::splitmix64 splitmix64;
        const auto size = latinSquare.size();
        std::vector<uint_fast16_t> transversal;
        transversal.reserve(size);

        uint_fast8_t counter = 0;

        while (transversal.size() < size) {
            auto& region = latinSquare.randomMinEntropyRegion();

            if (region.entropy() > 0) {
                counter = 0;

                const auto regionIndex = region.index();
                const auto cells = region.enabledCells();
                auto cell = cells[splitmix64.next() % cells.size()];
                const auto cellIndex = cell->index();
                transversal.emplace_back(cellIndex);
                auto indexes = latinSquare.relatedToChosenCellIndexes(cell);
                latinSquare.disable(indexes);
                latinSquare.decrease(indexes, cellIndex);
                latinSquare.disable(cellIndex);

                updateHistory_.emplace_back(cellIndex, indexes);

                if (checkAddToBacktrackingHistory(cellIndex)) {
                    backtrackingHistory_.emplace_back(regionIndex, cellIndex);
                }
            } else {
                if (++counter > 1) {
                    auto backtrackingData = backtrackingHistory_.back();
                    backtrackingHistory_.pop_back();

                    std::vector<uint_fast16_t> indexes;
                    indexes.reserve(size);
                    indexes.emplace_back(backtrackingData.cellIndex());
                    const auto regionIndex = backtrackingData.regionIndex();

                    while (checkRemoveFromBacktrackingHistory(regionIndex)) {
                        backtrackingData = backtrackingHistory_.back();
                        backtrackingHistory_.pop_back();

                        indexes.emplace_back(backtrackingData.cellIndex());
                    }

                    latinSquare.enable(indexes);
                    latinSquare.increase(indexes);

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                transversal.pop_back();

                const auto updateData = updateHistory_.back();
                updateHistory_.pop_back();

                const auto indexes = updateData.indexes();
                latinSquare.enable(indexes);
                latinSquare.increase(indexes);
                latinSquare.enable(updateData.index());
            }
        }

        return transversal;
    }

    const boost::multiprecision::mpz_int Generator::count(LatinSquare::LatinSquare& latinSquare) {
        const auto size = latinSquare.size();
        std::vector<uint_fast16_t> transversal;
        transversal.reserve(size);

        boost::multiprecision::mpz_int transversalsCounter = 0;
        uint_fast8_t counter = 0;

        while (true) {
            if (transversal.size() < size) {
                auto& region = latinSquare.minEntropyRegion();

                if (region.entropy() > 0) {
                    counter = 0;

                    const auto regionIndex = region.index();
                    auto cell = region.enabledCells()[0];
                    const auto cellIndex = cell->index();
                    transversal.emplace_back(cellIndex);
                    auto indexes = latinSquare.relatedToChosenCellIndexes(cell);
                    latinSquare.disable(indexes);
                    latinSquare.decrease(indexes, cellIndex);
                    latinSquare.disable(cellIndex);

                    updateHistory_.emplace_back(cellIndex, indexes);

                    if (checkAddToBacktrackingHistory(cellIndex)) {
                        backtrackingHistory_.emplace_back(regionIndex, cellIndex);
                    }
                } else {
                    if (++counter > 1) {
                        auto backtrackingData = backtrackingHistory_.back();
                        backtrackingHistory_.pop_back();

                        std::vector<uint_fast16_t> indexes;
                        indexes.reserve(size);
                        indexes.emplace_back(backtrackingData.cellIndex());
                        const auto& regionIndex = backtrackingData.regionIndex();

                        while (checkRemoveFromBacktrackingHistory(regionIndex)) {
                            backtrackingData = backtrackingHistory_.back();
                            backtrackingHistory_.pop_back();

                            indexes.emplace_back(backtrackingData.cellIndex());
                        }

                        latinSquare.enable(indexes);
                        latinSquare.increase(indexes);

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    transversal.pop_back();

                    const auto updateData = updateHistory_.back();
                    updateHistory_.pop_back();

                    const auto indexes = updateData.indexes();
                    latinSquare.enable(indexes);
                    latinSquare.increase(indexes);
                    latinSquare.enable(updateData.index());
                }
            } else {
                ++transversalsCounter;
                ++counter;

                transversal.pop_back();

                const auto updateData = updateHistory_.back();
                updateHistory_.pop_back();

                const auto indexes = updateData.indexes();
                latinSquare.enable(indexes);
                latinSquare.increase(indexes);
                latinSquare.enable(updateData.index());
            }
        }

        return transversalsCounter;
    }
}
