#include "Region.hpp"

#include <algorithm>

#include <cpp/iterator.hpp>

namespace LatinSquare {
    Region::Region(
        const std::string& id, const int latinSquareSize, const std::vector<std::reference_wrapper<Cell>> cells)
        : id_(id), latinSquareSize_(latinSquareSize), entropy_(latinSquareSize_), cells_(cells), enabled_(true) {
        setIdAsInt(id);
    }

    const std::string& Region::getId() const {
        return id_;
    }

    int Region::getIdAsInt() const {
        return idAsInt_;
    }

    void Region::setIdAsInt(const std::string& id) {
        idAsInt_ = id[0] == 'R' ? 0 : latinSquareSize_;
        idAsInt_ <<= (id[0] == '#');
        idAsInt_ += std::atoi(id.c_str() + 1);
    }

    int Region::getEntropy() const {
        return entropy_;
    }

    void Region::decreaseEntropyBy(const int number) {
        entropy_ -= number;
    }

    void Region::increaseEntropyBy(const int number) {
        entropy_ += number;
    }

    const std::vector<std::reference_wrapper<Cell>> Region::getEnabledCells() const {
        std::vector<std::reference_wrapper<Cell>> cells;
        cells.reserve(latinSquareSize_);

        std::copy_if(cells_.cbegin(), cells_.cend(), cpp::back_emplacer(cells),
                     [](const auto& cellRef) { return cellRef.get().isEnabled(); });

        return cells;
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
