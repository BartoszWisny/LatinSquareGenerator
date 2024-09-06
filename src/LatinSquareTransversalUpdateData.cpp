#include "LatinSquareTransversalUpdateData.hpp"

namespace LatinSquareGenerator {
    TransversalUpdateData::TransversalUpdateData(
        const Cell& chosenCell, const std::set<std::string>& disabledCellsIds) {
        setChosenCellId(chosenCell.getId());
        setDisabledCellsIds(disabledCellsIds);
    }


    const std::string& TransversalUpdateData::getChosenCellId() const {
        return chosenCellId_;
    }

    void TransversalUpdateData::setChosenCellId(const std::string& chosenCellId) {
        chosenCellId_ = chosenCellId;
    }

    const std::set<std::string>& TransversalUpdateData::getDisabledCellsIds() const {
        return disabledCellsIds_;
    }

    void TransversalUpdateData::setDisabledCellsIds(const std::set<std::string>& disabledCellsIds) {
        disabledCellsIds_ = disabledCellsIds;
    }
}
