#include "EntropyData.hpp"

#include <algorithm>
#include <iterator>

namespace LatinSquare {
    int EntropyData::getEntropy() const {
        return entropy_;
    }

    void EntropyData::setEntropy(const int entropy) {
        entropy_ = entropy;
    }

    const std::set<int>& EntropyData::getRemainingNumbers() const {
        return remainingNumbers_;
    }

    void EntropyData::setRemainingNumbers(const std::set<int>& remainingNumbers) {
        remainingNumbers_ = remainingNumbers;
    }

    void EntropyData::setMaxEntropy(const int maxEntropy) {
        maxEntropy_ = maxEntropy;
    }

    void EntropyData::resetEntropyData() {
        std::set<int> remainingNumbers;
        std::generate_n(std::inserter(remainingNumbers, remainingNumbers.cend()), maxEntropy_,
                        [number = 0]() mutable { return ++number; });

        setEntropy(maxEntropy_);
        setRemainingNumbers(remainingNumbers);
    }

    void EntropyData::clearEntropyData() {
        setEntropy(0);
        remainingNumbers_.clear();
    }

    bool EntropyData::removeRemainingNumber(const int number) {
        const auto result = remainingNumbers_.erase(number);

        if (result) {
            --entropy_;
        }

        return result;
    }

    void EntropyData::restoreRemainingNumber(const int number) {
        ++entropy_;
        remainingNumbers_.insert(number);
    }
}
