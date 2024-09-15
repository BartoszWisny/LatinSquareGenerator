#pragma once

#include <vector>

namespace LatinSquare {
    class EntropyData {
        public:
            int getEntropy() const;
            const std::vector<int>& getRemainingNumbers() const;

            void setMaxEntropy(const int maxEntropy);

            void resetEntropyData();
            void clearEntropyData();

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setEntropy(const int entropy);
            void setRemainingNumbers(const std::vector<int>& remainingNumbers);

            int entropy_;
            std::vector<int> remainingNumbers_;

            int maxEntropy_;
    };
}
