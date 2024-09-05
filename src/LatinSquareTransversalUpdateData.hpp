// #pragma once

// #include <set>
// #include <string>

// #include "LatinSquareCell.hpp"
// #include "LatinSquareEntropyData.hpp"

// namespace LatinSquareGenerator {
//     class TransversalUpdateData {
//         public:
//             TransversalUpdateData(const Cell& chosenCell, const std::set<std::string>& disabledCellsIds);






//             const std::string& getFilledCellId() const;
//             int getFilledCellNumber() const;
//             const EntropyData& getPreviousEntropyData() const;
//             const std::set<std::string>& getUpdatedCellsIds() const;

//         private:
//             void setFilledCellId(const std::string& filledCellId);
//             void setFilledCellNumber(const int number);
//             void setPreviousEntropyData(const EntropyData& previousEntropyData);
//             void setUpdatedCellsIds(const std::set<std::string>& updatedCellsIds);

//             std::string filledCellId_;
//             int filledCellNumber_;
//             EntropyData previousEntropyData_;
//             std::set<std::string> updatedCellsIds_;
//     };
// }
