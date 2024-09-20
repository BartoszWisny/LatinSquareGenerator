#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Cell.hpp"

namespace LatinSquare {
    class Region {
        public:
            Region(const std::string& id, const int latinSquareSize,
                const std::vector<std::reference_wrapper<Cell>> cells);

            const std::string& getId() const;
            int getIdAsInt() const;
            int getEntropy() const;
            void decreaseEntropyBy(const int number);
            void increaseEntropyBy(const int number);
            const std::vector<std::reference_wrapper<Cell>> getEnabledCells() const;
            bool isEnabled() const;
            void enable();
            void disable();

        private:
            void setIdAsInt(const std::string& id);

            std::string id_;
            int idAsInt_, latinSquareSize_, entropy_;
            std::vector<std::reference_wrapper<Cell>> cells_;
            bool enabled_;
    };
}
