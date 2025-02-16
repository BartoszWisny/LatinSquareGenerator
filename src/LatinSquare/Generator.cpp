#include "Generator.hpp"

namespace LatinSquare {
    // set max iterations (pruning) to prevent from program getting stuck (for big squares it is possible)
    const LatinSquare Generator::random(const uint_fast8_t size, const Type type) noexcept {
        cpp::splitmix64 splitmix64;
        LatinSquare latinSquare(size, type, splitmix64);

        std::shared_ptr<Cell> cell;
        uint_fast16_t index;
        std::vector<uint_fast8_t> numbers;
        uint_fast8_t number;
        EntropyData entropyData;
        std::vector<uint_fast16_t> indexes;
        BacktrackingData backtrackingData;
        UpdateData updateData;

        uint_fast16_t gridSize = size;
        gridSize *= size;
        updateHistory_.reserve(gridSize);
        backtrackingHistory_.reserve(gridSize);

        uint_fast16_t counter = 0;

        // uint_fast64_t it = 0;

        while (latinSquare.notFilled()) {
            // ++it;

            cell = latinSquare.randomMinEntropyCell();

            if (cell->entropy() > 0) {
                counter = 0;

                index = cell->index();
                numbers = cell->numbers();
                number = numbers[splitmix64.next() % numbers.size()];
                entropyData = cell->entropyData();
                latinSquare.fill(index, number);
                indexes = latinSquare.relatedToFilledCellIndexes(cell);
                latinSquare.update(indexes, number);

                updateHistory_.emplace_back(index, number, entropyData, indexes);

                if (checkBacktrackingHistory(index)) {
                    backtrackingHistory_.emplace_back(index, entropyData);
                }
            } else {
                if (++counter > 1) {
                    backtrackingData = backtrackingHistory_.back();
                    backtrackingHistory_.pop_back();

                    latinSquare.set(backtrackingData.index(), backtrackingData.entropyData());

                    if (updateHistory_.empty()) {
                        break;
                    }
                }

                updateData = updateHistory_.back();
                updateHistory_.pop_back();

                latinSquare.clear(updateData.index(), updateData.entropyData());
                latinSquare.restore(updateData.indexes(), updateData.number());
            }
        }

        // std::cout << "Iterations: " << it << std::endl;

        return latinSquare;
    }

    const boost::multiprecision::mpz_int Generator::count(const uint_fast8_t size) {
        LatinSquare latinSquare(size, Type::Reduced);

        std::shared_ptr<Cell> cell;
        uint_fast16_t index;
        uint_fast8_t number;
        EntropyData entropyData;
        std::vector<uint_fast16_t> indexes;
        BacktrackingData backtrackingData;
        UpdateData updateData;

        uint_fast16_t gridSize = size;
        gridSize *= size;
        updateHistory_.reserve(gridSize);
        backtrackingHistory_.reserve(gridSize);

        boost::multiprecision::mpz_int latinSquaresCounter = 0;
        uint_fast16_t counter = 0;

        while (true) {
            if (latinSquare.notFilled()) {
                cell = latinSquare.minEntropyCell();

                if (cell->entropy() > 0) {
                    counter = 0;

                    index = cell->index();
                    number = cell->numbers()[0];
                    entropyData = cell->entropyData();
                    latinSquare.fill(index, number);
                    indexes = latinSquare.relatedToFilledCellIndexes(cell);
                    latinSquare.update(indexes, number);

                    updateHistory_.emplace_back(index, number, entropyData, indexes);

                    if (checkBacktrackingHistory(index)) {
                        backtrackingHistory_.emplace_back(index, entropyData);
                    }
                } else {
                    if (++counter > 1) {
                        backtrackingData = backtrackingHistory_.back();
                        backtrackingHistory_.pop_back();

                        latinSquare.set(backtrackingData.index(), backtrackingData.entropyData());

                        if (updateHistory_.empty()) {
                            break;
                        }
                    }

                    updateData = updateHistory_.back();
                    updateHistory_.pop_back();

                    latinSquare.clear(updateData.index(), updateData.entropyData());
                    latinSquare.restore(updateData.indexes(), updateData.number());
                }
            } else {
                ++latinSquaresCounter;
                ++counter;

                updateData = updateHistory_.back();
                updateHistory_.pop_back();

                latinSquare.clear(updateData.index(), updateData.entropyData());
                latinSquare.restore(updateData.indexes(), updateData.number());
            }
        }

        return latinSquaresCounter;
    }
}
