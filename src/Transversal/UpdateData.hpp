#pragma once

#include <cstdint>
#include <vector>

namespace Transversal {
    class UpdateData {
        public:
            inline constexpr explicit UpdateData(const uint_fast8_t regionIndex,
                const uint_fast16_t cellIndex, const std::vector<uint_fast16_t>& indexes) noexcept
                : regionIndex_(regionIndex), cellIndex_(cellIndex), indexes_(indexes) {}

            UpdateData(const UpdateData&) = default;
            UpdateData& operator=(const UpdateData&) = default;

            UpdateData(UpdateData&&) noexcept = default;
            UpdateData& operator=(UpdateData&&) noexcept = default;

            [[nodiscard]] inline constexpr uint_fast8_t regionIndex() const noexcept{
                return regionIndex_;
            }

            [[nodiscard]] inline constexpr uint_fast16_t cellIndex() const noexcept{
                return cellIndex_;
            }

            [[nodiscard]] inline const std::vector<uint_fast16_t>& indexes() const noexcept {
                return indexes_;
            }

        private:
            uint_fast8_t regionIndex_;
            uint_fast16_t cellIndex_;
            std::vector<uint_fast16_t> indexes_;
    };
}
