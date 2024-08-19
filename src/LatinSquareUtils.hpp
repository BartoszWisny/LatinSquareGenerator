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

            const std::set<int>& getRemainingNumbers() const;
            void resetRemainingNumbers();
            void clearRemainingNumbers();

            void setMaxEntropy(const int maxEntropy);

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setEntropy(const int entropy);

            void setRemainingNumbers(const std::set<int>& remainingNumbers);

            int entropy_;
            std::set<int> remainingNumbers_;

            int maxEntropy_;
    };

    class FilledCellData {
        public:
            FilledCellData();
            FilledCellData(const std::string& id, const int number, const EntropyData& previousEntropyData);

            const std::string& getId() const;

            int getNumber() const;

            const EntropyData& getPreviousEntropyData() const;

        private:
            void setId(const std::string& id);

            void setNumber(const int number);

            void setPreviousEntropyData(const EntropyData& previousEntropyData);

            std::string id_;
            int number_;
            EntropyData previousEntropyData_;
    };

    class UpdateData {
        public:
            UpdateData(); // maybe ref to filled cell + refs to updated cells? finally it can be done!!!
            UpdateData(const FilledCellData& filledCellData, const std::set<std::string>& updatedCellsIds);

            const FilledCellData& getFilledCellData() const;

            const std::set<std::string>& getUpdatedCellsIds() const;

        private:
            void setFilledCellData(const FilledCellData& filledCellData);

            void setUpdatedCellsIds(const std::set<std::string>& updatedCellsIds);

            FilledCellData filledCellData_;
            std::set<std::string> updatedCellsIds_;
    };
}
