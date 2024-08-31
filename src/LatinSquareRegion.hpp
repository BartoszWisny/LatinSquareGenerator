#pragma once

#include <functional>
#include <vector>

#include "LatinSquareCell.hpp"

namespace LatinSquareGenerator {
    class Region {
        public:
            Region(const std::string& id, const int entropy, const std::vector<std::reference_wrapper<Cell>>& cells);

            const std::string& getId() const;
            int getEntropy() const;
            const std::vector<std::reference_wrapper<Cell>>& getCells() const;
            void setCells(const std::vector<std::reference_wrapper<Cell>>& cells);
            bool isAvailable() const;
            void setAvailable(const bool available);

        private:
            void setId(const std::string& id);
            void setEntropy(const int entropy);

            std::string id_;
            int entropy_;
            std::vector<std::reference_wrapper<Cell>> cells_;
            bool available_;
    };
}
