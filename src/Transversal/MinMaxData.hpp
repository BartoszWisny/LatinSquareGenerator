#pragma once

#include <utility>

#include <boost/multiprecision/gmp.hpp>

#include "LatinSquare/LatinSquare.hpp"

namespace Transversal {
    class MinMaxData {
        public:
            inline explicit MinMaxData(
                const boost::multiprecision::mpz_int& counter, const LatinSquare::LatinSquare& latinSquare) noexcept
                : counter_(counter), latinSquare_(latinSquare) {}




            MinMaxData(const MinMaxData& other)
                : counter_(other.counter_), latinSquare_(other.latinSquare_) {}

            MinMaxData& operator=(const MinMaxData& other) {
                if (this != &other) {
                    counter_ = other.counter_;
                    latinSquare_ = other.latinSquare_;
                }

                return *this;
            }

            // MinMaxData(MinMaxData&& other) noexcept {
            //     counter_ = std::exchange(other.counter_, 0);
            //     latinSquare_ = std::move(latinSquare_);
            // }

            // MinMaxData& operator=(MinMaxData&& other) noexcept {
            //     if (this != &other) {
            //         counter_ = std::exchange(other.counter_, 0);
            //         latinSquare_ = std::move(latinSquare_);
            //     }

            //     return *this;
            // }





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
