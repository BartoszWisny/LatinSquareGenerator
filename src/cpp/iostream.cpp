#include <iostream.hpp>

namespace cpp {
    const auto BOLD_ON = "\x1b[1m";
    const auto GREEEN_ON = "\x1b[38;5;46m";
    const auto RESET = "\x1b[0;0m";

    const std::string bold_on() {
        return BOLD_ON;
    }

    const std::string green_on() {
        return GREEEN_ON;
    }

    const std::string reset() {
        return RESET;
    }
}
