#pragma once

#include <set>
#include <string>

namespace LatinSquareGenerator {
    class EntropyData {
        public:
            EntropyData();

            int getEntropy() const;
            void resetEntropy();
            void clearEntropy();

            std::set<int> getRemainingNumbers();
            void resetRemainingNumbers();
            void clearRemainingNumbers();

            void setMaxEntropy(int maxEntropy);

            bool removeRemainingNumber(int number);
            void restoreRemainingNumber(int number);

        private:
            void setEntropy(int entropy);

            void setRemainingNumbers(std::set<int> remainingNumbers);

            int entropy_;
            std::set<int> remainingNumbers_;

            int maxEntropy_;
    };

    class FilledCellData {
        public:
            FilledCellData();
            FilledCellData(const std::string& id, const int number, const EntropyData& previousEntropyData);

            std::string getId();

            int getNumber();

            EntropyData getPreviousEntropyData();

        private:
            void setId(std::string id);

            void setNumber(int number);

            void setPreviousEntropyData(EntropyData previousEntropyData);

            std::string id_;
            int number_;
            EntropyData previousEntropyData_;
    };

    class UpdateData {
        public:
            UpdateData(); // maybe ref to filled cell + refs to updated cells?
            UpdateData(FilledCellData filledCellData, std::set<std::string> updatedCellsIds);

            FilledCellData getFilledCellData();

            std::set<std::string> getUpdatedCellsIds();

        private:
            void setFilledCellData(FilledCellData filledCellData);

            void setUpdatedCellsIds(std::set<std::string> updatedCellsIds);

            FilledCellData filledCellData_;
            std::set<std::string> updatedCellsIds_;
    };
}
