#include "LatinSquareTransversalBacktrackingData.hpp"

namespace LatinSquareGenerator {
    TransversalBacktrackingData::TransversalBacktrackingData(const Region& region, const Cell& cell) {
        setRegionId(region.getId());
        setChosenCellId(cell.getId());
    }

    const std::string& TransversalBacktrackingData::getRegionId() const {
        return regionId_;
    }

    void TransversalBacktrackingData::setRegionId(const std::string& id) {
        regionId_ = id;
    }

    const std::string& TransversalBacktrackingData::getChosenCellId() const {
        return chosenCellId_;
    }

    void TransversalBacktrackingData::setChosenCellId(const std::string& id) {
        chosenCellId_ = id;
    }
}
