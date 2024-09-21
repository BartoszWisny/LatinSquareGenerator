#include "UpdateData.hpp"

namespace Transversal {
    UpdateData::UpdateData(const LatinSquare::Cell& cell, const std::vector<std::string>& ids)
        : chosenCellId_(cell.getId()), disabledCellsIds_(ids) {}


    const std::string& UpdateData::getChosenCellId() const {
        return chosenCellId_;
    }

    const std::vector<std::string>& UpdateData::getDisabledCellsIds() const {
        return disabledCellsIds_;
    }
}
