#include "LatinSquareRegion.hpp"

namespace LatinSquareGenerator {
    Region::Region(const std::string& id, const int entropy, const std::vector<std::reference_wrapper<Cell>>& cells) {
        setId(id);
        setEntropy(entropy);
        setCells(cells);
        setAvailable(true);
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

    const std::vector<std::reference_wrapper<Cell>>& Region::getCells() const {
        return cells_;
    }

    void Region::setCells(const std::vector<std::reference_wrapper<Cell>>& cells) {
        cells_ = cells;
    }

    bool Region::isAvailable() const {
        return available_;
    }

    void Region::setAvailable(const bool available) {
        available_ = available;
    }
}
