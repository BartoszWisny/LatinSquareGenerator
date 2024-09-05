#include "LatinSquare.hpp"

#include <algorithm>
#include <format>
#include <iterator>
#include <map>
#include <string_view>

namespace LatinSquareGenerator {
    LatinSquare::LatinSquare() {}

    LatinSquare::LatinSquare(const int size, const std::mt19937& mersenneTwister) {
        setSize(size);
        setMersenneTwister(mersenneTwister);
        reset();
        setRegions();
        shuffleGrid();
    }

    int LatinSquare::getSize() const {
        return size_;
    }

    void LatinSquare::setSize(const int size) {
        size_ = size;
        gridSize_ = size * size;
    }

    void LatinSquare::sortGridByRows() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      if (firstCell.getRow() == secondCell.getRow()) {
                          return firstCell.getColumn() < secondCell.getColumn();
                      } else {
                          return firstCell.getRow() < secondCell.getRow();
                      }
                  });
    }

    void LatinSquare::sortGridByColumns() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      if (firstCell.getColumn() == secondCell.getColumn()) {
                          return firstCell.getRow() < secondCell.getRow();
                      } else {
                          return firstCell.getColumn() < secondCell.getColumn();
                      }
                  });
    }

    void LatinSquare::sortGridByNumbers() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      return firstCell.getNumber() < secondCell.getNumber();
                  });
    }

    const std::vector<Cell>& LatinSquare::getGrid() {
        return grid_;
    }

    const std::vector<Region>& LatinSquare::getRegions() const {
        return regions_;
    }

    void LatinSquare::setRegions() {
        std::vector<std::function<void(void)>> sortingTypes =
            {[this]() { sortGridByRows(); }, [this]() { sortGridByColumns(); }, [this]() { sortGridByNumbers(); }};
        std::vector<std::string> indexFormats = {"R{}", "C{}", "#{}"};

        for (int type = 0; type < 3; type++) {
            sortingTypes.at(type);
            auto iterator = grid_.cbegin();
            int index = 0;

            while (iterator != grid_.cend()) {
                std::vector<std::reference_wrapper<Cell>> cells;
                std::copy(iterator, iterator + size_, cells.begin());
                regions_.emplace_back(
                    Region(std::vformat(indexFormats.at(type), std::make_format_args(++index)), size_, cells));
                std::advance(iterator, size_);
            }
        }
    }

    void LatinSquare::setMersenneTwister(const std::mt19937& mersenneTwister) {
        mersenneTwister_ = mersenneTwister;
    }

    void LatinSquare::reset() {
        if (grid_.empty()) {
            for (int index = 0; index < gridSize_; index++) {
                grid_.emplace_back(Cell(index / size_ + 1, index % size_ + 1, size_));
            }
        } else {
            std::for_each(grid_.begin(), grid_.end(), [](auto& cell) { cell.reset(); });
        }
    }

    void LatinSquare::shuffleGrid() {
        std::shuffle(grid_.begin(), grid_.end(), mersenneTwister_);
    }

    // consider using map for cells - key should be cell id
    Cell& LatinSquare::getCell(const std::string& id) {
        const auto& iterator = std::find_if(
            grid_.begin(), grid_.end(), [&id](const auto& cell) { return cell.getId() == id; });

        return *iterator;
    }

    const std::vector<std::reference_wrapper<Cell>> LatinSquare::getCells(const std::set<std::string>& ids) {
        std::vector<std::reference_wrapper<Cell>> cells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(cells),
                     [this, &ids](const auto& cell) { return ids.contains(cell.getId()); });

        return cells;
    }

    const std::vector<std::reference_wrapper<Region>> LatinSquare::getRegions(const std::set<std::string>& ids) {
        std::vector<std::reference_wrapper<Region>> regions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(regions),
                     [this, &ids](const auto& region) { return ids.contains(region.getId()); });

        return regions;
    }

    bool LatinSquare::checkIfNotFilledCellExists() const {
        return std::any_of(grid_.cbegin(), grid_.cend(), [](const auto& cell) { return cell.getNumber() == 0; });
    }

    Cell& LatinSquare::getNotFilledCellWithMinimumEntropy() {
        std::sort(grid_.begin(), grid_.end(),
                  [](const auto& firstCell, const auto& secondCell) {
                      return firstCell.getEntropy() < secondCell.getEntropy();
                  });
        const auto& iterator = std::find_if(
            grid_.begin(), grid_.end(), [](const auto& cell) { return cell.getNumber() == 0; });

        return *iterator;
    }

    bool LatinSquare::checkIfRelatedToFilledCell(const Cell& filledCell, const Cell& cell) const {
        return (filledCell.getRow() == cell.getRow()) != (filledCell.getColumn() == cell.getColumn());
    }

    const std::set<std::string> LatinSquare::getUpdatedCellsIds(const Cell& filledCell) {
        std::vector<std::reference_wrapper<Cell>> updatedCells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(updatedCells),
                     [this, &filledCell](const auto& cell) { return checkIfRelatedToFilledCell(filledCell, cell); });
        std::erase_if(updatedCells,
                      [&filledCell](const auto& cellRef) {
                          return !cellRef.get().removeRemainingNumber(filledCell.getNumber());
                      });
        std::set<std::string> updatedCellsIds;
        std::transform(updatedCells.cbegin(), updatedCells.cend(),
                       std::inserter(updatedCellsIds, updatedCellsIds.cend()),
                       [](const auto& cellRef) { return cellRef.get().getId(); });

        return updatedCellsIds;
    }

    const UpdateData LatinSquare::getUpdateData(const Cell& filledCell, const EntropyData& previousEntropyData) {
        return UpdateData(filledCell, previousEntropyData, getUpdatedCellsIds(filledCell));
    }

    // check these functions

    Region& LatinSquare::getEnabledRegionWithMinimumEntropy() {
        std::sort(regions_.begin(), regions_.end(),
                  [](const auto& firstRegion, const auto& secondRegion) {
                      return firstRegion.getEntropy() < secondRegion.getEntropy();
                  });
        const auto& iterator = std::find_if(
            regions_.begin(), regions_.end(), [](const auto& region) { return region.isEnabled(); });

        return *iterator;
    }

    bool LatinSquare::checkIfRelatedToChosenCell(const Cell& chosenCell, const Cell& cell) const {
        return chosenCell.isEnabled() && (chosenCell.getRow() == cell.getRow()
               || chosenCell.getColumn() == cell.getColumn() || chosenCell.getNumber() == cell.getNumber());
    }

    void LatinSquare::decreaseRelatedRegionsEntropy(
        const std::set<std::string>& relatedRegionsIds, const std::map<std::string, int>& entropyUpdates) {
        auto relatedRegions = getRegions(relatedRegionsIds);

        for (const auto& regionRef : relatedRegions) {
            auto& region = regionRef.get();
            region.decreaseEntropyBy(entropyUpdates.at(region.getId()));
        }
    }

    const std::set<std::string> LatinSquare::getDisabledCellsIds(const Cell& chosenCell) {
        std::vector<std::reference_wrapper<Cell>> relatedCells;
        std::copy_if(grid_.begin(), grid_.end(), std::back_inserter(relatedCells),
                     [this, &chosenCell](const auto& cell) { return checkIfRelatedToChosenCell(chosenCell, cell); });
        std::set<std::string> relatedRegionsIds;
        std::map<std::string, int> entropyUpdates;

        for (const auto& cellRef : relatedCells) {
            auto& cell = cellRef.get();
            cell.disable();

            relatedRegionsIds.insert({cell.getRowId(), cell.getColumnId(), cell.getNumberId()});
            ++entropyUpdates.at(cell.getRowId());
            ++entropyUpdates.at(cell.getColumnId());
            ++entropyUpdates.at(cell.getNumberId());
        }

        decreaseRelatedRegionsEntropy(relatedRegionsIds, entropyUpdates);

        std::set<std::string> relatedCellsIds;
        std::transform(relatedCells.cbegin(), relatedCells.cend(),
                       std::inserter(relatedCellsIds, relatedCellsIds.cend()),
                       [](const auto& cellRef) { return cellRef.get().getId(); });

        return relatedCellsIds;
    }

    bool LatinSquare::checkIfRelatedRegion(const Cell& cell, const Region& region) const {
        return cell.getRowId() == region.getId() || cell.getColumnId() == region.getId()
               || cell.getNumberId() == region.getId();
    }

    void LatinSquare::disableRelatedRegions(const Cell& cell) {
        std::vector<std::reference_wrapper<Region>> relatedRegions;
        std::copy_if(regions_.begin(), regions_.end(), std::back_inserter(relatedRegions),
                     [this, &cell](const auto& region) { return checkIfRelatedRegion(cell, region); });

        for (const auto& relatedRegionRef : relatedRegions) {
            relatedRegionRef.get().disable();
        }
    }
}
