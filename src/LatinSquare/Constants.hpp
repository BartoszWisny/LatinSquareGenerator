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
        ReducedSymmetric = 0x04,
        ReducedCyclic = 0x05,

        // TODO: check if these types are needed
        ReducedDiagonal = 0x06,
        ReducedSuperSymmetric = 0x07
    };

    static constexpr uint_fast32_t MAX_ITERATIONS = 1000000;
    static constexpr uint_fast32_t SYMMETRIC_MAX_ITERATIONS = 2000000;

    static constexpr std::string_view TYPE_NORMAL = "N";
    static constexpr std::string_view TYPE_REDUCED = "R";
    static constexpr std::string_view TYPE_REDUCED_SYMMETRIC = "RS";
    static constexpr std::string_view TYPE_REDUCED_CYCLIC = "RC";

    // TODO: check if these types are needed
    static constexpr std::string_view TYPE_REDUCED_DIAGONAL = "RD";
    static constexpr std::string_view TYPE_REDUCED_SUPER_SYMMETRIC = "RSS";

    static constexpr std::string_view LATIN_SQUARES_RANDOM = "-lr";
    static constexpr std::string_view LATIN_SQUARES_FILE = "-lf";
    static constexpr std::string_view LATIN_SQUARES_COUNT = "-lc";
    static constexpr std::string_view LATIN_SQUARES_TEMPLATE_RANDOM = "-ltr";
    static constexpr std::string_view LATIN_SQUARES_TEMPLATE_FILE = "-ltf";
    static constexpr std::string_view LATIN_SQUARES_TEMPLATE_COUNT = "-ltc";

    static constexpr std::string_view SYMMETRIC_LATIN_SQUARES_RANDOM = "-slr";
    static constexpr std::string_view SYMMETRIC_LATIN_SQUARES_FILE = "-slf";
    static constexpr std::string_view SYMMETRIC_LATIN_SQUARES_COUNT = "-slc";
    static constexpr std::string_view SYMMETRIC_LATIN_SQUARES_TEMPLATE_RANDOM = "-sltr";
    static constexpr std::string_view SYMMETRIC_LATIN_SQUARES_TEMPLATE_FILE = "-sltf";
    static constexpr std::string_view SYMMETRIC_LATIN_SQUARES_TEMPLATE_COUNT = "-sltc";

    static constexpr std::string_view LATIN_SQUARES = "Number of latin squares: ";
    static constexpr std::string_view USAGE =
        "USAGE:\n"
        "Generate random latin square:\n"
        "-lr <size> <type>\n"
        "Generate random latin square to file:\n"
        "-lf <size> <type> <output>\n"
        "Find number of latin squares:\n"
        "-lc <size> <type>\n"
        "Generate random latin square for template:\n"
        "-ltr <input>\n"
        "Generate random latin square for template to file:\n"
        "-ltf <input> <output>\n"
        "Find number of latin squares for template:\n"
        "-ltc <input>\n\n"
        "Generate random symmetric latin square:\n"
        "-slr <size> <type>\n"
        "Generate random symmetric latin square to file:\n"
        "-slf <size> <type> <output>\n"
        "Find number of symmetric latin squares:\n"
        "-slc <size> <type>\n"
        "Generate symmetric random latin square for template:\n"
        "-sltr <input>\n"
        "Generate symmetric random latin square for template to file:\n"
        "-sltf <input> <output>\n"
        "Find number of symmetric latin squares for template:\n"
        "-sltc <input>\n\n"
        "Find random transversal in latin square:\n"
        "-tr <input>\n"
        "Find number of transversals in latin square:\n"
        "-tc <input>\n"
        "Find minimum and maximum number of transversals in latin squares:\n"
        "-tm <size> <type>\n"
        "Find minimum and maximum number of transversals in latin squares for template:\n"
        "-tt <input>\n\n"


        "<size> should be from 1 to 64\n"
        "<type> should be one of: N (normal), R (reduced), C (reduced cyclic), D (reduced diagonal), "
        "S (reduced super-symmetric)\n"
        "File <input> should contain latin square or template with <size> rows, <size> columns "
        "and numbers from 1 to <size> (or 0 for empty cells)\n";

    static constexpr std::string_view TIME = "Time: ";
    static constexpr std::string_view SECONDS = " seconds\n";
}
