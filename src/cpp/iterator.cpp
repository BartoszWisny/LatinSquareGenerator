// #include <iostream>
// #include <vector>
// #include <utility> // for std::forward

// // Custom back_emplacer to use emplace_back instead of push_back
// template <typename Container>
// class back_emplacer_iterator {
// public:
//     // Type aliases for iterator traits
//     using container_type = Container;
//     using value_type = void;
//     using difference_type = std::ptrdiff_t;
//     using pointer = void;
//     using reference = void;
//     using iterator_category = std::output_iterator_tag;

//     // Constructor takes a reference to the container
//     explicit back_emplacer_iterator(Container& c) : container(&c) {}

//     // Overload operator= to call emplace_back on the container
//     template <typename T>
//     back_emplacer_iterator& operator=(T&& value) {
//         container->emplace_back(std::forward<T>(value));
//         return *this;
//     }

//     // No-op increment operators for output iterators
//     back_emplacer_iterator& operator*() { return *this; }
//     back_emplacer_iterator& operator++() { return *this; }
//     back_emplacer_iterator operator++(int) { return *this; }

// private:
//     Container* container;  // Pointer to the container
// };

// // Helper function to create back_emplacer_iterator
// template <typename Container>
// back_emplacer_iterator<Container> back_emplacer(Container& c) {
//     return back_emplacer_iterator<Container>(c);
// }

// int main() {
//     std::vector<std::pair<int, std::string>> vec;

//     // Use back_emplacer to add elements using emplace_back
//     auto it = back_emplacer(vec);
//     *it = std::make_pair(1, "One");
//     *it = std::make_pair(2, "Two");

//     for (const auto& p : vec) {
//         std::cout << p.first << ": " << p.second << '\n';
//     }

//     return 0;
// }
