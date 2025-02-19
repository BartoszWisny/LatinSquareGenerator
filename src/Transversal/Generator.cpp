#include "Generator.hpp"

#include "LatinSquare/Region.hpp"

namespace Transversal {
    const std::vector<uint_fast16_t> Generator::random(LatinSquare::LatinSquare& latinSquare) {
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

        while (transversal.size() < latinSquare.size()) {
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
        }

        return transversal;
    }

    const boost::multiprecision::mpz_int Generator::count(LatinSquare::LatinSquare& latinSquare) {
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
}
