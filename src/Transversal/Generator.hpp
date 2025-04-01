#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <boost/multiprecision/gmp.hpp>

#include "BacktrackingData.hpp"
#include "LatinSquare/BacktrackingData.hpp"
#include "LatinSquare/Constants.hpp"
#include "LatinSquare/LatinSquare.hpp"
#include "LatinSquare/SymmetricLatinSquare.hpp"
#include "LatinSquare/UpdateData.hpp"
#include "MinMaxData.hpp"
#include "SymmetricBacktrackingData.hpp"
#include "SymmetricMinMaxData.hpp"
#include "SymmetricUpdateData.hpp"
#include "UpdateData.hpp"

namespace Transversal {
    class Generator {
        public:
            [[nodiscard]] const std::vector<uint_fast16_t> random(LatinSquare::LatinSquare& latinSquare) noexcept;

            [[nodiscard]] const boost::multiprecision::mpz_int count(
                LatinSquare::LatinSquare& latinSquare) noexcept;
            [[nodiscard]] const std::array<MinMaxData, 2> minMax(
                const uint_fast8_t size, const LatinSquare::Type type) noexcept;
            [[nodiscard]] const std::array<MinMaxData, 2> minMax(LatinSquare::LatinSquare& latinSquare) noexcept;

            [[nodiscard]] const std::vector<uint_fast16_t> symmetricRandom(
                LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;

            [[nodiscard]] const boost::multiprecision::mpz_int symmetricCount(
                LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;
            [[nodiscard]] const std::array<SymmetricMinMaxData, 2> symmetricMinMax(
                const uint_fast8_t size, const LatinSquare::Type type) noexcept;
            [[nodiscard]] const std::array<SymmetricMinMaxData, 2> symmetricMinMax(
                LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;

        private:
            [[nodiscard]] constexpr boost::multiprecision::mpz_int factorial(const uint_fast8_t size) noexcept;

            std::vector<UpdateData> updateHistory_;
            std::vector<BacktrackingData> backtrackingHistory_;
            std::vector<SymmetricUpdateData> symmetricUpdateHistory_;
            std::vector<SymmetricBacktrackingData> symmetricBacktrackingHistory_;
            std::vector<LatinSquare::UpdateData> latinSquareUpdateHistory_;
            std::vector<LatinSquare::BacktrackingData> latinSquareBacktrackingHistory_;
    };
}
