#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Cell.hpp"

namespace LatinSquare {
    class Region {
        public:
            Region(const std::string& id, const int entropy, const std::vector<std::reference_wrapper<Cell>> cells);

            const std::string& getId() const;
            int getEntropy() const;
            void decreaseEntropyBy(const int number);
            void increaseEntropyBy(const int number);
            const std::vector<std::reference_wrapper<Cell>>& getCells() const;
            const std::vector<std::reference_wrapper<Cell>> getEnabledCells() const;
            void setCells(const std::vector<std::reference_wrapper<Cell>>& cells);
            bool isEnabled() const;
            void enable();
            void disable();

        private:
            void setId(const std::string& id);
            void setEntropy(const int entropy);

            std::string id_;
            int entropy_;
            std::vector<std::reference_wrapper<Cell>> cells_;
            bool enabled_;
    };
}
