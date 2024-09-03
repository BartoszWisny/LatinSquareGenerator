#include "LatinSquareRegion.hpp"

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

    void Region::decreaseEntropyBy(int number) {
        entropy_ -= number;
    }

    void Region::increaseEntropyBy(int number) {
        entropy_ += number;
    }

    const std::vector<std::reference_wrapper<Cell>>& Region::getCells() const {
        return cells_;
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
        // entropy_ = 0;
        enabled_ = false;
    }
}
