#pragma once

#include <boost/multiprecision/gmp.hpp>

#include "LatinSquare/LatinSquare.hpp"

namespace Transversal {
    class MinMaxData {
        public:
            inline explicit MinMaxData(const boost::multiprecision::mpz_int& counter,
                const boost::multiprecision::mpz_int& latinSquaresCounter,
                const LatinSquare::LatinSquare& latinSquare) noexcept
                : counter_(counter), latinSquaresCounter_(latinSquaresCounter), latinSquare_(latinSquare) {}

            MinMaxData(const MinMaxData&) = default;
            MinMaxData& operator=(const MinMaxData&) = default;

            MinMaxData(MinMaxData&&) noexcept = default;
            MinMaxData& operator=(MinMaxData&&) noexcept = default;

            [[nodiscard]] inline const boost::multiprecision::mpz_int& counter() const noexcept {
                return counter_;
            }

            [[nodiscard]] inline const boost::multiprecision::mpz_int& latinSquaresCounter() const noexcept {
                return latinSquaresCounter_;
            }

            [[nodiscard]] inline const LatinSquare::LatinSquare& latinSquare() const noexcept {
                return latinSquare_;
            }

            inline constexpr void set(const boost::multiprecision::mpz_int& counter) noexcept {
                counter_ = counter;
            }

            inline constexpr void reset() noexcept {
                latinSquaresCounter_ = 1;
            }

            inline constexpr void increase() noexcept {
                ++latinSquaresCounter_;
            }

            inline constexpr void set(const LatinSquare::LatinSquare& latinSquare) noexcept {
                latinSquare_ = latinSquare;
            }

        private:
            boost::multiprecision::mpz_int counter_;
            boost::multiprecision::mpz_int latinSquaresCounter_;
            LatinSquare::LatinSquare latinSquare_;
    };
}
