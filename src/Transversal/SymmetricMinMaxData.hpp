#pragma once

#include <boost/multiprecision/gmp.hpp>

#include "LatinSquare/SymmetricLatinSquare.hpp"

namespace Transversal {
    class SymmetricMinMaxData {
        public:
            inline explicit SymmetricMinMaxData(const boost::multiprecision::mpz_int& counter,
                const LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept
                : counter_(counter), symmetricLatinSquare_(symmetricLatinSquare) {}

            SymmetricMinMaxData(const SymmetricMinMaxData&) = default;
            SymmetricMinMaxData& operator=(const SymmetricMinMaxData&) = default;

            SymmetricMinMaxData(SymmetricMinMaxData&&) noexcept = default;
            SymmetricMinMaxData& operator=(SymmetricMinMaxData&&) noexcept = default;

            [[nodiscard]] inline const boost::multiprecision::mpz_int& counter() const noexcept {
                return counter_;
            }

            [[nodiscard]] inline const LatinSquare::SymmetricLatinSquare& symmetricLatinSquare() const noexcept {
                return symmetricLatinSquare_;
            }

            inline constexpr void set(const boost::multiprecision::mpz_int& counter) noexcept {
                counter_ = counter;
            }

            inline constexpr void set(const LatinSquare::SymmetricLatinSquare& symmetricLatinSquare) noexcept {
                symmetricLatinSquare_ = symmetricLatinSquare;
            }

        private:
            boost::multiprecision::mpz_int counter_;
            LatinSquare::SymmetricLatinSquare symmetricLatinSquare_;
    };
}
