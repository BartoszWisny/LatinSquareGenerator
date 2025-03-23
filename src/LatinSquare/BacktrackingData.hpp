#pragma once

#include <cstdint>
#include <utility>

#include "EntropyData.hpp"

namespace LatinSquare {
    class BacktrackingData {
        public:
            inline constexpr explicit BacktrackingData(
                const uint_fast16_t index, const EntropyData& entropyData) noexcept
                : index_(index), entropyData_(entropyData) {}

            BacktrackingData(const BacktrackingData& other)
                : index_(other.index_), entropyData_(other.entropyData_) {}

            BacktrackingData& operator=(const BacktrackingData& other) {
                if (this != &other) {
                    index_ = other.index_;
                    entropyData_ = other.entropyData_;
                }

                return *this;
            }

            BacktrackingData(BacktrackingData&& other) noexcept {
                index_ = std::exchange(other.index_, 0);
                entropyData_ = std::move(other.entropyData_);
            }

            BacktrackingData& operator=(BacktrackingData&& other) noexcept {
                if (this != &other) {
                    index_ = other.index_;
                    entropyData_ = std::move(other.entropyData_);
                }

                return *this;
            }

            [[nodiscard]] inline constexpr uint_fast16_t index() const noexcept {
                return index_;
            }

            [[nodiscard]] inline const EntropyData& entropyData() const noexcept {
                return entropyData_;
            }

        private:
            uint_fast16_t index_;
            EntropyData entropyData_;
    };
}
