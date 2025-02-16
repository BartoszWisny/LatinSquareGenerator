#pragma once

#include <cstdint>
#include <vector>

#include <boost/multiprecision/gmp.hpp>

#include "BacktrackingData.hpp"
#include "LatinSquare/LatinSquare.hpp"
#include "UpdateData.hpp"

namespace Transversal {
    class Generator {
        public:
            [[nodiscard]] const std::vector<uint_fast16_t> random(LatinSquare::LatinSquare& latinSquare);

            [[nodiscard]] const boost::multiprecision::mpz_int count(LatinSquare::LatinSquare& latinSquare);

        private:
            [[nodiscard]] inline constexpr bool checkAddToBacktrackingHistory(
                const uint_fast16_t index) const noexcept {
                return backtrackingHistory_.empty() || index != backtrackingHistory_.back().cellIndex();
            }

            [[nodiscard]] inline constexpr bool checkRemoveFromBacktrackingHistory(
                const uint_fast8_t index) const noexcept {
                return !backtrackingHistory_.empty() && index == backtrackingHistory_.back().regionIndex();
            }

            std::vector<UpdateData> updateHistory_;
            std::vector<BacktrackingData> backtrackingHistory_;
    };
}
