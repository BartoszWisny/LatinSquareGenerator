#include <string.hpp>

namespace cpp {
    const std::string repeat(const std::string& string, int repeats) {
        std::string repeated;
        repeated.reserve(repeats * string.size());

        while (repeats--) {
            repeated += string;
        }

        return repeated;
    }
}
