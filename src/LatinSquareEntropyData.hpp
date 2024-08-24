#pragma once

#include <set>

namespace LatinSquareGenerator {
    class EntropyData {
        public:
            int getEntropy() const;
            const std::set<int>& getRemainingNumbers() const;

            void setMaxEntropy(const int maxEntropy);

            void resetEntropyData();
            void clearEntropyData();

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setEntropy(const int entropy);
            void setRemainingNumbers(const std::set<int>& remainingNumbers);

            int entropy_;
            std::set<int> remainingNumbers_;

            int maxEntropy_;
    };
}
