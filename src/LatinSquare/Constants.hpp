#pragma once

#include <cstdint>
#include <string_view>

namespace LatinSquare {
    static constexpr uint_fast8_t EMPTY = 0xFF;
    static constexpr uint_fast8_t MAX_SIZE = 64;

    enum class Type : uint_fast8_t {
        Unknown = 0x00,
        Normal = 0x01,
        Custom = 0x02,
        Reduced = 0x03,
        ReducedCyclic = 0x04,
        ReducedDiagonal = 0x05,
        ReducedSuperSymmetric = 0x06
    };

    static constexpr uint_fast32_t MAX_ITERATIONS = 1000000;

    static constexpr std::string_view TYPE_NORMAL = "N";
    static constexpr std::string_view TYPE_REDUCED = "R";
    static constexpr std::string_view TYPE_REDUCED_CYCLIC = "C";
    static constexpr std::string_view TYPE_REDUCED_DIAGONAL = "D";
    static constexpr std::string_view TYPE_REDUCED_SUPER_SYMMETRIC = "S";

    static constexpr std::string_view LATIN_SQUARES_RANDOM = "-lr";
    static constexpr std::string_view LATIN_SQUARES_FILE = "-lf";
    static constexpr std::string_view LATIN_SQUARES_COUNT = "-lc";
    static constexpr std::string_view LATIN_SQUARES_TEMPLATE = "-lt";

    static constexpr std::string_view LATIN_SQUARES = "Number of latin squares: ";
    static constexpr std::string_view USAGE =
        "USAGE:\n"
        "Generate random latin square:\n"
        "-lr <size> <type>\n"
        "Generate random latin square to file:\n"
        "-lf <size> <type> <filename>\n"
        "Find number of latin squares:\n"
        "-lc <size> <type>\n"
        "Find number of latin squares for template:\n"
        "-lt <size> <filename>\n"
        "Find random transversal in latin square:\n"
        "-tr <size> <filename>\n"
        "Find number of transversals in latin square:\n"
        "-tc <size> <filename>\n"
        "Find minimum and maximum number of transversals in latin squares:\n"
        "-tm <size> <type>\n"
        "<size> should be from 1 to 64\n"
        "<type> should be one of: N (normal), R (reduced), C (reduced cyclic), D (reduced diagonal), "
        "S (reduced super-symmetric)\n"
        "File <filename> should contain latin square or template with <size> rows, <size> columns "
        "and numbers from 1 to <size> (or 0 for empty cells)";

    static constexpr std::string_view TIME = "Time: ";
    static constexpr std::string_view SECONDS = " seconds";
}
