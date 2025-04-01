#pragma once

#include <cstdint>
#include <vector>
#include <utility>

namespace Transversal {
    class UpdateData {
        public:
            inline constexpr explicit UpdateData(
                const uint_fast16_t index, const std::vector<uint_fast16_t>& indexes) noexcept
                : index_(index), indexes_(indexes) {}

            UpdateData(const UpdateData& other)
                : index_(other.index_), indexes_(other.indexes_) {}

            UpdateData& operator=(const UpdateData& other) {
                if (this != &other) {
                    index_ = other.index_;
                    indexes_ = other.indexes_;
                }

                return *this;
            }

            UpdateData(UpdateData&& other) noexcept {
                index_ = std::exchange(other.index_, 0);
                indexes_ = std::move(other.indexes_);
            }

            UpdateData& operator=(UpdateData&& other) noexcept {
                if (this != &other) {
                    index_ = other.index_;
                    indexes_ = std::move(other.indexes_);
                }

                return *this;
            }

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
