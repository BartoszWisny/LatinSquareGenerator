#include <iostream.hpp>

namespace cpp {
    const auto BOLD_ON = "\x1b[1m";
    const auto GREEEN_ON = "\x1b[38;5;46m";
    const auto RESET = "\x1b[0;0m";

    std::ostream& bold_on(std::ostream& ostream) {
        return ostream << BOLD_ON;
    }

    std::ostream& green_on(std::ostream& ostream) {
        return ostream << GREEEN_ON;
    }

    std::ostream& reset(std::ostream& ostream) {
        return ostream << RESET;
    }
}
