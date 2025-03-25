#pragma once

#include <boost/multiprecision/gmp.hpp>

#include "LatinSquare/LatinSquare.hpp"

namespace Transversal {
    class MinMaxData {
        public:
            inline explicit MinMaxData(
                const boost::multiprecision::mpz_int& counter, const LatinSquare::LatinSquare& latinSquare) noexcept
                : counter_(counter), latinSquare_(latinSquare) {}

            [[nodiscard]] inline const boost::multiprecision::mpz_int& counter() const noexcept {
                return counter_;
            }

            [[nodiscard]] inline const LatinSquare::LatinSquare& latinSquare() const noexcept {
                return latinSquare_;
            }

            inline constexpr void set(const boost::multiprecision::mpz_int& counter) noexcept {
                counter_ = counter;
            }

            inline constexpr void set(const LatinSquare::LatinSquare& latinSquare) noexcept {
                latinSquare_ = latinSquare;
            }

        private:
            boost::multiprecision::mpz_int counter_;
            LatinSquare::LatinSquare latinSquare_;
    };
}
