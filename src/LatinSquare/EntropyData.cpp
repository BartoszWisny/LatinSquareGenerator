#include "EntropyData.hpp"

#include <algorithm>

namespace LatinSquare {
    int EntropyData::getEntropy() const {
        return entropy_;
    }

    inline void EntropyData::setEntropy(const int entropy) {
        entropy_ = entropy;
    }

    const std::vector<int>& EntropyData::getRemainingNumbers() const {
        return remainingNumbers_;
    }

    inline void EntropyData::setRemainingNumbers(const std::vector<int>& remainingNumbers) {
        remainingNumbers_ = remainingNumbers;
    }

    void EntropyData::setMaxEntropy(const int maxEntropy) {
        maxEntropy_ = maxEntropy;
    }

    void EntropyData::resetEntropyData() {
        std::vector<int> remainingNumbers;
        remainingNumbers.reserve(maxEntropy_);

        for (int number = 1; number <= maxEntropy_; ++number) {
            remainingNumbers.emplace_back(number);
        }

        setEntropy(maxEntropy_);
        setRemainingNumbers(remainingNumbers);
    }

    void EntropyData::clearEntropyData() {
        setEntropy(0);
        remainingNumbers_.clear();
    }

    bool EntropyData::removeRemainingNumber(const int number) {
        auto iterator = std::find(remainingNumbers_.cbegin(), remainingNumbers_.cend(), number);

        if (iterator != remainingNumbers_.cend()) {
            remainingNumbers_.erase(iterator);
            --entropy_;

            return true;
        }

        return false;
    }

    void EntropyData::restoreRemainingNumber(const int number) {
        remainingNumbers_.emplace_back(number);
        ++entropy_;
    }
}
