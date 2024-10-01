#include "EntropyData.hpp"

#include <algorithm>

namespace LatinSquare {
    EntropyData::EntropyData() {}

    EntropyData::EntropyData(const int maxEntropy)
        : maxEntropy_(maxEntropy) {}

    int EntropyData::getEntropy() const {
        return entropy_;
    }

    void EntropyData::setEntropy(const int entropy) {
        entropy_ = entropy;
    }

    // const boost::dynamic_bitset<>& EntropyData::getRemainingNumbers() const {
    //     return remainingNumbers_;
    // }

    const std::vector<int>& EntropyData::getRemainingNumbers() const {
        return remainingNumbers_;
    }

    void EntropyData::resetEntropyData() {
        setEntropy(maxEntropy_);

        // remainingNumbers_.resize(maxEntropy_);
        // remainingNumbers_.set();

        remainingNumbers_.reserve(maxEntropy_);

        for (int number = 0; number < maxEntropy_; ++number) {
            remainingNumbers_.emplace_back(number);
        }
    }

    void EntropyData::clearEntropyData() {
        setEntropy(0);
        // remainingNumbers_.reset();

        remainingNumbers_.clear();
    }

    bool EntropyData::removeRemainingNumber(const int number) {
        // if (remainingNumbers_[number - 1]) {
        //     remainingNumbers_.reset(number - 1);

        //     return true;
        // }

        // return false;

        auto iterator = std::find(remainingNumbers_.cbegin(), remainingNumbers_.cend(), number);

        if (iterator != remainingNumbers_.cend()) {
            remainingNumbers_.erase(iterator);
            --entropy_;

            return true;
        }

        return false;
    }

    void EntropyData::restoreRemainingNumber(const int number) {
        // remainingNumbers_.set(number - 1);

        remainingNumbers_.emplace_back(number);
        ++entropy_;
    }
}
