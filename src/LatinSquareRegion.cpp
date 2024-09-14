#include "LatinSquareRegion.hpp"

#include <algorithm>

namespace LatinSquareGenerator {
    Region::Region(const std::string& id, const int entropy, const std::vector<std::reference_wrapper<Cell>>& cells) {
        setId(id);
        setEntropy(entropy);
        setCells(cells);
        enable();
    }

    const std::string& Region::getId() const {
        return id_;
    }

    void Region::setId(const std::string& id) {
        id_ = id;
    }

    int Region::getEntropy() const {
        return entropy_;
    }

    void Region::setEntropy(const int entropy) {
        entropy_ = entropy;
    }

    void Region::decreaseEntropyBy(const int number) {
        entropy_ -= number;
    }

    void Region::increaseEntropyBy(const int number) {
        entropy_ += number;
    }

    const std::vector<std::reference_wrapper<Cell>>& Region::getCells() const {
        return cells_;
    }

    const std::vector<std::reference_wrapper<Cell>> Region::getEnabledCells() const {
        std::vector<std::reference_wrapper<Cell>> cells;
        std::copy_if(cells_.cbegin(), cells_.cend(), std::back_inserter(cells),
                     [](const auto& cell) { return cell.get().isEnabled(); });

        return cells;
    }

    void Region::setCells(const std::vector<std::reference_wrapper<Cell>>& cells) {
        cells_ = cells;
    }

    bool Region::isEnabled() const {
        return enabled_;
    }

    void Region::enable() {
        enabled_ = true;
    }

    void Region::disable() {
        enabled_ = false;
    }
}
