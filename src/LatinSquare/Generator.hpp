#pragma once

#include <cstdint>
#include <vector>

#include <boost/multiprecision/gmp.hpp>

#include "BacktrackingData.hpp"
#include "Constants.hpp"
#include "LatinSquare.hpp"
#include "UpdateData.hpp"

namespace LatinSquare {
    class Generator {
        public:
            [[nodiscard]] const LatinSquare random(const uint_fast8_t size, const Type type) noexcept;

            [[nodiscard]] const boost::multiprecision::mpz_int count(const uint_fast8_t size);

        private:
            std::vector<UpdateData> updateHistory_;
            std::vector<BacktrackingData> backtrackingHistory_;
    };
}
