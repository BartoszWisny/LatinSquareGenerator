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
            std::vector<UpdateData> updateHistory_;
            std::vector<BacktrackingData> backtrackingHistory_;
    };
}
