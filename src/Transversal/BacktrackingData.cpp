#include "BacktrackingData.hpp"

namespace Transversal {
    BacktrackingData::BacktrackingData(const LatinSquare::Region& region, const LatinSquare::Cell& cell) {
        setRegionId(region.getId());
        setChosenCellId(cell.getId());
    }

    const std::string& BacktrackingData::getRegionId() const {
        return regionId_;
    }

    void BacktrackingData::setRegionId(const std::string& id) {
        regionId_ = id;
    }

    const std::string& BacktrackingData::getChosenCellId() const {
        return chosenCellId_;
    }

    void BacktrackingData::setChosenCellId(const std::string& id) {
        chosenCellId_ = id;
    }
}
