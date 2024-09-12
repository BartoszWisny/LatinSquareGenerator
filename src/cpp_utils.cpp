#include <cpp_utils.hpp>

namespace cpp_utils {
    std::ostream& bold_on(std::ostream& ostream) {
        return ostream << "\x1b[1m";
    }

    std::ostream& green_on(std::ostream& ostream) {
        return ostream << "\x1b[38;5;46m";
    }

    std::ostream& reset(std::ostream& ostream) {
        return ostream << "\x1b[0;0m";
    }
}
