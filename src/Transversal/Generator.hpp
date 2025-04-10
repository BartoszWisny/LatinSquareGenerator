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

            [[nodiscard]] const boost::multiprecision::mpz_int count(LatinSquare::LatinSquare& latinSquare) noexcept;
            [[nodiscard]] const std::vector<MinMaxData>& minMax(
                const uint_fast8_t size, const LatinSquare::Type type) noexcept;
            [[nodiscard]] const std::vector<MinMaxData>& minMax(LatinSquare::LatinSquare& latinSquare) noexcept;

            [[nodiscard]] const std::vector<uint_fast16_t> symmetricRandom(
                LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;

            [[nodiscard]] const boost::multiprecision::mpz_int symmetricCount(
                LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;
            [[nodiscard]] const std::vector<SymmetricMinMaxData>& symmetricMinMax(
                const uint_fast8_t size, const LatinSquare::Type type) noexcept;
            [[nodiscard]] const std::vector<SymmetricMinMaxData>& symmetricMinMax(
                LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;

            // [[nodiscard]] const std::vector<uint_fast16_t> symmetricTriangularRandom(
            //     LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;

            [[nodiscard]] const boost::multiprecision::mpz_int symmetricTriangularCount(
                LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;
            [[nodiscard]] const std::vector<SymmetricMinMaxData>& symmetricTriangularMinMax(
                const uint_fast8_t size, const LatinSquare::Type type) noexcept;
            [[nodiscard]] const std::vector<SymmetricMinMaxData>& symmetricTriangularMinMax(
                LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept;

        private:
            [[nodiscard]] constexpr boost::multiprecision::mpz_int factorial(const uint_fast8_t size) noexcept;

            uint_fast8_t transversalSize_;
            uint_fast8_t almostSize_;
            uint_fast16_t cellIndex_;
            uint_fast8_t regionIndex_;
            boost::multiprecision::mpz_int transversalsCounter_;
            uint_fast8_t counter_;
            std::vector<UpdateData> updateHistory_;
            std::vector<BacktrackingData> backtrackingHistory_;
            std::vector<SymmetricUpdateData> symmetricUpdateHistory_;
            std::vector<SymmetricBacktrackingData> symmetricBacktrackingHistory_;
            std::vector<LatinSquare::UpdateData> latinSquareUpdateHistory_;
            std::vector<LatinSquare::BacktrackingData> latinSquareBacktrackingHistory_;
            std::vector<MinMaxData> latinSquaresCounters_;
            std::vector<SymmetricMinMaxData> symmetricLatinSquaresCounters_;
    };
}
