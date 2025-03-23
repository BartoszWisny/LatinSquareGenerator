#pragma once

#include <cstdint>
#include <vector>
#include <utility>

#include "EntropyData.hpp"

namespace LatinSquare {
    class UpdateData {
        public:
            inline constexpr explicit UpdateData(const uint_fast16_t index, const uint_fast8_t number,
                const EntropyData& entropyData, const std::vector<uint_fast16_t>& indexes) noexcept
                : index_(index), number_(number), entropyData_(entropyData), indexes_(indexes) {}

            UpdateData(const UpdateData& other)
                : index_(other.index_), number_(other.number_), entropyData_(other.entropyData_),
                  indexes_(other.indexes_) {}

            UpdateData& operator=(const UpdateData& other) {
                if (this != &other) {
                    index_ = other.index_;
                    number_ = other.number_;
                    entropyData_ = other.entropyData_;
                    indexes_ = other.indexes_;
                }

                return *this;
            }

            UpdateData(UpdateData&& other) noexcept {
                index_ = std::exchange(other.index_, 0);
                number_ = std::exchange(other.number_, 0);
                entropyData_ = std::move(other.entropyData_);
                indexes_ = std::move(other.indexes_);
            }

            UpdateData& operator=(UpdateData&& other) noexcept {
                if (this != &other) {
                    index_ = other.index_;
                    number_ = other.number_;
                    entropyData_ = std::move(other.entropyData_);
                    indexes_ = std::move(other.indexes_);
                }

                return *this;
            }

            [[nodiscard]] inline constexpr uint_fast16_t index() const noexcept{
                return index_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t number() const noexcept {
                return number_;
            }

            [[nodiscard]] inline const EntropyData& entropyData() const noexcept {
                return entropyData_;
            }

            [[nodiscard]] inline const std::vector<uint_fast16_t>& indexes() const noexcept {
                return indexes_;
            }

        private:
            uint_fast16_t index_;
            uint_fast8_t number_;
            EntropyData entropyData_;
            std::vector<uint_fast16_t> indexes_;
    };
}
