#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <boost/multiprecision/gmp.hpp>

#include "BacktrackingData.hpp"
#include "LatinSquare/BacktrackingData.hpp"
#include "LatinSquare/Constants.hpp"
#include "LatinSquare/LatinSquare.hpp"
#include "LatinSquare/UpdateData.hpp"
#include "UpdateData.hpp"

namespace Transversal {
    class Generator {
        public:
            [[nodiscard]] const std::vector<uint_fast16_t> random(LatinSquare::LatinSquare& latinSquare) noexcept;

            [[nodiscard]] const boost::multiprecision::mpz_int count(
                LatinSquare::LatinSquare& latinSquare) noexcept;

            [[nodiscard]] const std::array<boost::multiprecision::mpz_int, 2> minMax(
                const uint_fast8_t size, const LatinSquare::Type type) noexcept;

        private:
            [[nodiscard]] constexpr boost::multiprecision::mpz_int factorial(const uint_fast8_t size) noexcept;

            std::vector<UpdateData> updateHistory_;
            std::vector<BacktrackingData> backtrackingHistory_;
            std::vector<LatinSquare::UpdateData> latinSquareUpdateHistory_;
            std::vector<LatinSquare::BacktrackingData> latinSquareBacktrackingHistory_;
    };
}
