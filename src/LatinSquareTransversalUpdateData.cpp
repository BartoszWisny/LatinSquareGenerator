#include "LatinSquareTransversalUpdateData.hpp"

namespace LatinSquareGenerator {
    TransversalUpdateData::TransversalUpdateData(const Cell& cell, const std::set<std::string>& ids) {
        setChosenCellId(cell.getId());
        setDisabledCellsIds(ids);
    }


    const std::string& TransversalUpdateData::getChosenCellId() const {
        return chosenCellId_;
    }

    void TransversalUpdateData::setChosenCellId(const std::string& id) {
        chosenCellId_ = id;
    }

    const std::set<std::string>& TransversalUpdateData::getDisabledCellsIds() const {
        return disabledCellsIds_;
    }

    void TransversalUpdateData::setDisabledCellsIds(const std::set<std::string>& ids) {
        disabledCellsIds_ = ids;
    }
}
