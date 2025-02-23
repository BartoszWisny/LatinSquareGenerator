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

            // Cell(const Cell&) = delete;
            // Cell& operator=(const Cell&) = delete;
            Cell(const Cell&) = default;
            Cell& operator=(const Cell&) = default;

            Cell(Cell&& other) noexcept {
                index_ = std::exchange(other.index_, 0);
                rawRow_ = std::exchange(other.rawRow_, 0);
                rawColumn_ = std::exchange(other.rawColumn_, 0);
                number_ = std::exchange(other.number_, 0);
                size_ = std::exchange(other.size_, 0);
                entropyData_ = std::move(other.entropyData_);
                maxNumber_ = std::exchange(other.maxNumber_, 0);
                regionRow_ = std::exchange(other.regionRow_, 0);
                regionColumn_ = std::exchange(other.regionColumn_, 0);
                regionNumber_ = std::exchange(other.regionNumber_, 0);
                rowColumnSum_ = std::exchange(other.rowColumnSum_, 0);
                enabled_ = std::exchange(other.enabled_, false);
            }

            Cell& operator=(Cell&& other) noexcept {
                if (this != &other) {
                    index_ = std::exchange(other.index_, 0);
                    rawRow_ = std::exchange(other.rawRow_, 0);
                    rawColumn_ = std::exchange(other.rawColumn_, 0);
                    number_ = std::exchange(other.number_, 0);
                    size_ = std::exchange(other.size_, 0);
                    entropyData_ = std::move(other.entropyData_);
                    maxNumber_ = std::exchange(other.maxNumber_, 0);
                    regionRow_ = std::exchange(other.regionRow_, 0);
                    regionColumn_ = std::exchange(other.regionColumn_, 0);
                    regionNumber_ = std::exchange(other.regionNumber_, 0);
                    rowColumnSum_ = std::exchange(other.rowColumnSum_, 0);
                    enabled_ = std::exchange(other.enabled_, false);
                }

                return *this;
            }

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

            [[nodiscard]] inline constexpr bool filled() const noexcept {
                return number_ != EMPTY;
            }

            [[nodiscard]] inline constexpr bool notFilled() const noexcept {
                return number_ == EMPTY;
            }

            [[nodiscard]] inline constexpr bool enabled() const noexcept {
                return enabled_;
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

            inline constexpr void fill(const uint_fast8_t number) noexcept {
                number_ = number;
                regionNumber_ = number_;
                regionNumber_ += (size_ << 1);
                entropyData_.clear();
            }

            inline constexpr void clear(EntropyData entropyData) noexcept {
                entropyData.remove(number_);
                number_ = EMPTY;
                regionNumber_ = EMPTY;
                entropyData_ = entropyData;
            }

            inline constexpr bool remove(const uint_fast8_t number) noexcept {
                return entropyData_.remove(number);
            }

            inline constexpr void restore(const uint_fast8_t number) noexcept {
                entropyData_.restore(number);
            }

            [[nodiscard]] inline constexpr uint_fast8_t delta() const noexcept {
                return (rowColumnSum_ + size_ - number_) % size_;
            }

            [[nodiscard]] const std::string id() const noexcept;

        private:
            constexpr void reset(const Type type) noexcept;

            uint_fast16_t index_;
            uint_fast8_t rawRow_;
            uint_fast8_t rawColumn_;
            uint_fast8_t number_;
            uint_fast8_t size_;
            EntropyData entropyData_;
            uint_fast8_t maxNumber_;
            uint_fast8_t regionRow_;
            uint_fast8_t regionColumn_;
            uint_fast8_t regionNumber_;
            uint_fast8_t rowColumnSum_;
            bool enabled_;
    };
}
