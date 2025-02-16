#include "Region.hpp"

namespace LatinSquare {
    const std::vector<std::shared_ptr<Cell>> Region::enabledCells() const noexcept {
        std::vector<std::shared_ptr<Cell>> cells;
        cells.reserve(size_);

        for (const auto& cell : cells_) {
            if (cell->enabled()) {
                cells.emplace_back(cell);
            }
        }

        return cells;
    }
}
