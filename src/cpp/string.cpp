#include <string.hpp>

namespace cpp {
    const std::string repeat(const std::string& string, uint_fast8_t repeats) {
        std::string result;
        result.reserve(repeats * string.size());

        while (repeats--) {
            result.append(string);
        }

        return result;
    }
}
