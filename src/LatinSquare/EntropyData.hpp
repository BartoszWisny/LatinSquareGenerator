#pragma once

#include <vector>

// #include <boost/dynamic_bitset.hpp>

namespace LatinSquare {
    class EntropyData {
        public:
            EntropyData();
            EntropyData(const int maxEntropy);

            int getEntropy() const;
            // const boost::dynamic_bitset<>& getRemainingNumbers() const;
            const std::vector<int>& getRemainingNumbers() const;

            void resetEntropyData();
            void clearEntropyData();

            bool removeRemainingNumber(const int number);
            void restoreRemainingNumber(const int number);

        private:
            void setEntropy(const int entropy);

            int entropy_, maxEntropy_;

            // boost::dynamic_bitset<> remainingNumbers_;
            std::vector<int> remainingNumbers_;
    };
}
