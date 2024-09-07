#include "LatinSquareTransversalBacktrackingData.hpp"

namespace LatinSquareGenerator {
    TransversalBacktrackingData::TransversalBacktrackingData(const Region& region, const Cell& chosenCell) {
        setRegionId(region.getId());
        setChosenCellId(chosenCell.getId());
    }

    const std::string& TransversalBacktrackingData::getRegionId() const {
        return regionId_;
    }

    void TransversalBacktrackingData::setRegionId(const std::string& regionId) {
        regionId_ = regionId;
    }

    const std::string& TransversalBacktrackingData::getChosenCellId() const {
        return chosenCellId_;
    }

    void TransversalBacktrackingData::setChosenCellId(const std::string& chosenCellId) {
        chosenCellId_ = chosenCellId;
    }
}
