#include "UpdateData.hpp"

namespace Transversal {
    UpdateData::UpdateData(const LatinSquare::Cell& cell, const std::set<std::string>& ids)
        : chosenCellId_(cell.getId()), disabledCellsIds_(ids) {}


    const std::string& UpdateData::getChosenCellId() const {
        return chosenCellId_;
    }

    const std::set<std::string>& UpdateData::getDisabledCellsIds() const {
        return disabledCellsIds_;
    }
}
