#pragma once

#include <cstdint>
#include <vector>

namespace Transversal {
    class UpdateData {
        public:
            inline constexpr explicit UpdateData(
                const uint_fast16_t index, const std::vector<uint_fast16_t>& indexes) noexcept
                : index_(index), indexes_(indexes) {}

            [[nodiscard]] inline constexpr uint_fast16_t index() const noexcept{
                return index_;
            }

            [[nodiscard]] inline const std::vector<uint_fast16_t>& indexes() const noexcept {
                return indexes_;
            }

        private:
            uint_fast16_t index_;
            std::vector<uint_fast16_t> indexes_;
    };
}
