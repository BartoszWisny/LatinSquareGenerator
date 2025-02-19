#include "EntropyData.hpp"

namespace LatinSquare {
    const std::vector<uint_fast8_t> EntropyData::numbers() const noexcept {
        std::vector<uint_fast8_t> numbers;
        numbers.reserve(std::popcount(numbers_));
        auto bits = numbers_;

        while (bits) {
            numbers.emplace_back(std::countr_zero(bits));
            bits &= (bits - 1);
        }

        return numbers;
    }
}
