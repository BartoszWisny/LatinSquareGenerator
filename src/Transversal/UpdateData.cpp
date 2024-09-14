#include "UpdateData.hpp"

namespace Transversal {
    UpdateData::UpdateData(const LatinSquare::Cell& cell, const std::set<std::string>& ids) {
        setChosenCellId(cell.getId());
        setDisabledCellsIds(ids);
    }


    const std::string& UpdateData::getChosenCellId() const {
        return chosenCellId_;
    }

    void UpdateData::setChosenCellId(const std::string& id) {
        chosenCellId_ = id;
    }

    const std::set<std::string>& UpdateData::getDisabledCellsIds() const {
        return disabledCellsIds_;
    }

    void UpdateData::setDisabledCellsIds(const std::set<std::string>& ids) {
        disabledCellsIds_ = ids;
    }
}
