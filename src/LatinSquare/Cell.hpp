#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

#include "Constants.hpp"
#include "EntropyData.hpp"

namespace LatinSquare {
    class Cell {
        public:
            explicit Cell(const uint_fast16_t index, const uint_fast8_t row, const uint_fast8_t column,
                const uint_fast8_t size, const Type type) noexcept;

            Cell(const Cell&) = default;
            Cell& operator=(const Cell&) = default;

            Cell(Cell&& other) noexcept = default;
            Cell& operator=(Cell&& other) noexcept = default;

            [[nodiscard]] inline constexpr uint_fast16_t index() const noexcept {
                return index_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t rawRow() const noexcept {
                return rawRow_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t rawColumn() const noexcept {
                return rawColumn_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t number() const noexcept{
                return number_;
            }

            [[nodiscard]] inline constexpr Type type() const noexcept{
                return type_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t entropy() const noexcept {
                return entropyData_.entropy();
            }

            [[nodiscard]] inline const std::vector<uint_fast8_t> numbers() const noexcept {
                return entropyData_.numbers();
            }

            [[nodiscard]] inline const EntropyData& entropyData() const noexcept {
                return entropyData_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t regionRow() const noexcept {
                return regionRow_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t regionColumn() const noexcept {
                return regionColumn_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t regionNumber() const noexcept {
                return regionNumber_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t rowColumnSum() const noexcept {
                return rowColumnSum_;
            }

            [[nodiscard]] inline constexpr bool filled() const noexcept {
                return number_ != EMPTY;
            }

            [[nodiscard]] inline constexpr bool notFilled() const noexcept {
                return number_ == EMPTY;
            }

            [[nodiscard]] inline constexpr bool enabled() const noexcept {
                return enabled_;
            }

            [[nodiscard]] inline constexpr bool notOnDiagonal() const noexcept {
                return notOnDiagonal_;
            }

            [[nodiscard]] inline constexpr bool notOnAntidiagonal() const noexcept {
                return notOnAntidiagonal_;
            }

            [[nodiscard]] inline constexpr uint_fast8_t otherRegionIndex(const uint_fast8_t regionIndex) noexcept {
                return otherRegionIndexes_[regionIndex != rawColumn_];
            }

            [[nodiscard]] inline constexpr bool triangularEnabled() noexcept {
                return triangularEnabled_[0] || triangularEnabled_[notOnDiagonal_];
            }

            [[nodiscard]] inline constexpr bool triangularEnabled(const uint_fast8_t regionIndex) noexcept {
                return triangularEnabled_[regionIndex != rawColumn_];
            }

            [[nodiscard]] inline constexpr bool otherTriangularEnabled(const uint_fast8_t regionIndex) noexcept {
                return triangularEnabled_[otherRegionIndex(regionIndex) != rawColumn_];
            }

            [[nodiscard]] inline constexpr bool columnTriangularEnabled() noexcept {
                return triangularEnabled_[0];
            }

            [[nodiscard]] inline constexpr bool rowTriangularEnabled() noexcept {
                return triangularEnabled_[notOnDiagonal_];
            }

            inline constexpr void set(const EntropyData& entropyData) noexcept {
                entropyData_ = entropyData;
            }

            inline constexpr void enable() noexcept {
                enabled_ = true;
            }

            inline constexpr void disable() noexcept {
                enabled_ = false;
            }

            inline constexpr void triangularEnable(const uint_fast8_t regionIndex) noexcept {
                triangularEnabled_[regionIndex != rawColumn_] = true;
            }

            inline constexpr void triangularDisable(const uint_fast8_t regionIndex) noexcept {
                triangularEnabled_[regionIndex != rawColumn_] = false;
            }

            inline constexpr void fillAndClear(const uint_fast8_t number) noexcept {
                number_ = number;
                entropyData_.clear();
            }

            inline constexpr void fill(const uint_fast8_t number) noexcept {
                number_ = number;
            }

            inline constexpr void setRegionNumber() noexcept {
                regionNumber_ = number_;
                regionNumber_ += doubleSize_;
            }

            inline constexpr void clearAndRemove(EntropyData entropyData) noexcept {
                entropyData.remove(number_);
                number_ = EMPTY;
                entropyData_ = entropyData;
            }

            inline constexpr void clear(const EntropyData& entropyData) noexcept {
                number_ = EMPTY;
                entropyData_ = entropyData;
            }

            inline constexpr void clear() noexcept {
                number_ = EMPTY;
            }

            [[nodiscard]] inline constexpr uint_fast64_t canBeRemoved(const uint_fast8_t number) noexcept {
                return entropyData_.canBeRemoved(number);
            }

            inline constexpr void remove(const uint_fast8_t number) noexcept {
                return entropyData_.remove(number);
            }

            inline constexpr void remove() noexcept {
                return entropyData_.remove();
            }

            inline constexpr void restore(const uint_fast8_t number) noexcept {
                entropyData_.restore(number);
            }

            void reset() noexcept;
            [[nodiscard]] const std::string id() const noexcept;

        private:
            uint_fast16_t index_;
            uint_fast8_t rawRow_;
            uint_fast8_t rawColumn_;
            uint_fast8_t number_;
            uint_fast8_t size_;
            uint_fast8_t doubleSize_;
            Type type_;
            EntropyData entropyData_;
            uint_fast8_t maxNumber_;
            uint_fast8_t regionRow_;
            uint_fast8_t regionColumn_;
            uint_fast8_t regionNumber_;
            uint_fast8_t rowColumnSum_;
            bool enabled_;
            bool notOnDiagonal_;
            bool notOnAntidiagonal_;
            std::vector<uint_fast8_t> otherRegionIndexes_;
            std::vector<bool> triangularEnabled_;
    };
}
