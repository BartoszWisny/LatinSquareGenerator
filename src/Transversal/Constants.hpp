#pragma once

#include <cstdint>
#include <string_view>

namespace Transversal {
    static constexpr uint_fast32_t MAX_ITERATIONS = 1000;

    static constexpr std::string_view TRANSVERSALS_RANDOM = "-tr";
    static constexpr std::string_view TRANSVERSALS_COUNT = "-tc";
    static constexpr std::string_view TRANSVERSALS_MINMAX = "-tm";
    static constexpr std::string_view TRANSVERSALS_TEMPLATE = "-tt";

    static constexpr std::string_view SYMMETRIC_TRANSVERSALS_RANDOM = "-str";
    static constexpr std::string_view SYMMETRIC_TRANSVERSALS_COUNT = "-stc";
    static constexpr std::string_view SYMMETRIC_TRANSVERSALS_MINMAX = "-stm";
    static constexpr std::string_view SYMMETRIC_TRANSVERSALS_TEMPLATE = "-stt";

    // static constexpr std::string_view SYMMETRIC_TRIANGULAR_TRANSVERSALS_RANDOM = "-sttr";
    static constexpr std::string_view SYMMETRIC_TRIANGULAR_TRANSVERSALS_COUNT = "-sttc";
    static constexpr std::string_view SYMMETRIC_TRIANGULAR_TRANSVERSALS_MINMAX = "-sttm";
    static constexpr std::string_view SYMMETRIC_TRIANGULAR_TRANSVERSALS_TEMPLATE = "-sttt";

    static constexpr std::string_view TRANSVERSALS = "Number of transversals: ";
    static constexpr std::string_view MIN_TRANSVERSALS = "Minimum number of transversals: ";
    static constexpr std::string_view LATIN_SQUARES_MIN_TRANSVERSALS =
        "Number of latin squares with minimum number of transversals: ";
    static constexpr std::string_view MAX_TRANSVERSALS = "Maximum number of transversals: ";
    static constexpr std::string_view LATIN_SQUARES_MAX_TRANSVERSALS =
        "Number of latin squares with maximum number of transversals: ";

    static constexpr std::string_view TIME = "Time: ";
    static constexpr std::string_view SECONDS = " seconds\n";
}
