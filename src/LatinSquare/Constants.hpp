#pragma once

#include <cstdint>
#include <string_view>

namespace LatinSquare {
    static constexpr uint_fast8_t EMPTY = 0xFF;
    static constexpr uint_fast8_t MAX_SIZE = 64;
    static constexpr uint_fast16_t DEFAULT_CELL_INDEX = 0xFFFF;
    static constexpr uint_fast8_t DEFAULT_REGION_INDEX = 0xFF;
    static constexpr uint_fast8_t DEFAULT_ENTROPY = 0xFF;
    static constexpr uint_fast8_t DEFAULT_NUMBER = 0xFF;

    enum class Type : uint_fast8_t {
        Unknown = 0x00,
        Normal = 0x01,
        Custom = 0x02,
        Reduced = 0x03,
        ReducedCyclic = 0x04,
        ReducedDiagonal = 0x05
    };

    static constexpr uint_fast32_t MAX_ITERATIONS = 1000000;
    static constexpr uint_fast32_t SYMMETRIC_MAX_ITERATIONS = 1000000;

    static constexpr std::string_view TYPE_NORMAL = "N";
    static constexpr std::string_view TYPE_REDUCED = "R";
    static constexpr std::string_view TYPE_REDUCED_CYCLIC = "C";
    static constexpr std::string_view TYPE_REDUCED_DIAGONAL = "D";

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
        "-ltr <template_input>\n"
        "Generate random latin square for template to file:\n"
        "-ltf <template_input> <output>\n"
        "Find number of latin squares for template:\n"
        "-ltc <template_input>\n\n"
        "Generate random symmetric latin square:\n"
        "-slr <size> <type>\n"
        "Generate random symmetric latin square to file:\n"
        "-slf <size> <type> <output>\n"
        "Find number of symmetric latin squares:\n"
        "-slc <size> <type>\n"
        "Generate symmetric random latin square for template:\n"
        "-sltr <triangular_input>\n"
        "Generate symmetric random latin square for template to file:\n"
        "-sltf <triangular_input> <output>\n"
        "Find number of symmetric latin squares for template:\n"
        "-sltc <triangular_input>\n\n"
        "Find random transversal in latin square:\n"
        "-tr <full_input>\n"
        "Find number of transversals in latin square:\n"
        "-tc <full_input>\n"
        "Find minimum and maximum number of transversals in latin squares:\n"
        "-tm <size> <type>\n"
        "Find minimum and maximum number of transversals in latin squares for template:\n"
        "-tt <template_input>\n\n"
        "Find random transversal in symmetric latin square:\n"
        "-str <triangular_input>\n"
        "Find number of transversals in symmetric latin square:\n"
        "-stc <triangular_input>\n"
        "Find minimum and maximum number of transversals in symmetric latin squares:\n"
        "-stm <size> <type>\n"
        "Find minimum and maximum number of transversals in symmetric latin squares for template:\n"
        "-stt <triangular_input>\n\n"
        // "Find random transversal in symmetric latin square using lower-triangular grid:\n"
        // "-sttr <triangular_input>\n"
        "Find number of transversals in symmetric latin square using lower-triangular grid:\n"
        "-sttc <triangular_input>\n"
        "Find minimum and maximum number of transversals in symmetric latin squares using lower-triangular grid:\n"
        "-sttm <size> <type>\n"
        "Find minimum and maximum number of transversals in symmetric latin squares for template "
        "using lower-triangular grid:\n"
        "-sttt <triangular_input>\n\n"
        "<size> should be number from 1 to 64\n"
        "<type> should be one of characters: N (normal), R (reduced), C (reduced cyclic), D (reduced diagonal)\n"
        "File <template_input> should contain template of latin square with <size> rows, <size> columns "
        "and numbers from 1 to <size> or 0 for empty cells\n"
        "File <triangular_input> should contain template of lower-triangular table of symmetric latin square "
        "with <size> rows, <size> columns and numbers from 1 to <size> or 0 for empty cells\n"
        "File <full_input> should contain latin square with <size> rows, <size> columns and numbers from 1 to <size>\n";

    static constexpr std::string_view TIME = "Time: ";
    static constexpr std::string_view SECONDS = " seconds\n";
}
