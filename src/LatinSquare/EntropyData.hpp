#pragma once

#include <vector>

namespace LatinSquare {
    class EntropyData {
        public:
            EntropyData();
            EntropyData(const int maxEntropy);

            int getEntropy() const;
            const std::vector<int>& getRemainingNumbers() const;

            void resetEntropyData();
            void clearEntropyData();

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setEntropy(const int entropy);
            void setRemainingNumbers(const std::vector<int>& remainingNumbers);

            int entropy_, maxEntropy_;
            std::vector<int> remainingNumbers_;
    };
}
