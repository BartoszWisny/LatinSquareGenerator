#pragma once

#include <cstdint>
#include <vector>

#include "SymmetricCellUpdateData.hpp"

namespace Transversal {
    class SymmetricUpdateData {
        public:
            inline constexpr explicit SymmetricUpdateData(const uint_fast8_t regionIndex,
                const std::vector<SymmetricCellUpdateData>& cellUpdateData,
                const std::vector<SymmetricCellUpdateData>& otherCellsUpdateData) noexcept
                : regionIndex_(regionIndex), cellUpdateData_(cellUpdateData),
                  otherCellsUpdateData_(otherCellsUpdateData) {}

            SymmetricUpdateData(const SymmetricUpdateData&) = default;
            SymmetricUpdateData& operator=(const SymmetricUpdateData&) = default;

            SymmetricUpdateData(SymmetricUpdateData&&) noexcept = default;
            SymmetricUpdateData& operator=(SymmetricUpdateData&&) noexcept = default;

            [[nodiscard]] inline constexpr uint_fast8_t regionIndex() const noexcept {
                return regionIndex_;
            }

            [[nodiscard]] inline const std::vector<SymmetricCellUpdateData>& cellUpdateData() const noexcept {
                return cellUpdateData_;
            }

            [[nodiscard]] inline const std::vector<SymmetricCellUpdateData>& otherCellsUpdateData() const noexcept {
                return otherCellsUpdateData_;
            }

        private:
            uint_fast8_t regionIndex_;
            std::vector<SymmetricCellUpdateData> cellUpdateData_;
            std::vector<SymmetricCellUpdateData> otherCellsUpdateData_;
    };
}
