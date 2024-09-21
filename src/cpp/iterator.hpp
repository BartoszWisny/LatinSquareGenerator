#pragma once

#include <cstddef>
#include <iterator>
#include <vector>
#include <utility>

namespace cpp {
    template <typename Container>
    class back_emplacer_iterator {
        public:
            using container_type = Container;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::output_iterator_tag;
            using pointer = void;
            using reference = void;
            using value_type = void;

            explicit back_emplacer_iterator(Container& container)
                : container_(&container) {}

            template <typename T>
            back_emplacer_iterator& operator=(T&& value) {
                container_->emplace_back(std::forward<T>(value));

                return *this;
            }

            back_emplacer_iterator& operator*() {
                return *this;
            }

            back_emplacer_iterator& operator++() {
                return *this;
            }

            back_emplacer_iterator operator++(int) {
                return *this;
            }

        private:
            Container* container_;
    };

    template <typename Container>
    back_emplacer_iterator<Container> back_emplacer(Container& container) {
        return back_emplacer_iterator<Container>(container);
    }
}
