#include "Main.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

#include "LatinSquare/Constants.hpp"
#include "LatinSquare/Generator.hpp"
#include "LatinSquare/Utils.hpp"
#include "Transversal/Constants.hpp"
#include "Transversal/Generator.hpp"
#include "Transversal/Utils.hpp"

void setup() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

int main(int argc, char* argv[]) {
    setup();
    auto latinSquareGenerator = LatinSquare::Generator();
    auto transversalGenerator = Transversal::Generator();

    if (argc == 4 && std::string_view(argv[1]).compare(LatinSquare::LATIN_SQUARES_RANDOM) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown) {
            const auto start = std::chrono::steady_clock::now();
            const auto latinSquare = latinSquareGenerator.random(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            LatinSquare::printBoard(latinSquare);
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 5 && std::string_view(argv[1]).compare(LatinSquare::LATIN_SQUARES_FILE) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);
        const std::string filename = argv[4];

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown
            && std::string_view(filename).compare("") != 0) {
            const auto start = std::chrono::steady_clock::now();
            const auto latinSquare = latinSquareGenerator.random(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            LatinSquare::printFile(latinSquare, filename);
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 4 && std::string_view(argv[1]).compare(LatinSquare::LATIN_SQUARES_COUNT) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown) {
            const auto start = std::chrono::steady_clock::now();
            const auto count = latinSquareGenerator.count(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            std::cout.write(LatinSquare::LATIN_SQUARES.data(), LatinSquare::LATIN_SQUARES.length());
            std::cout << count << std::endl;
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3 && std::string_view(argv[1]).compare(LatinSquare::LATIN_SQUARES_TEMPLATE_RANDOM) == 0) {
        const auto [size, numbers] = LatinSquare::convert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            const auto start = std::chrono::steady_clock::now();
            const auto latinSquare = latinSquareGenerator.random(size, numbers);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            LatinSquare::printBoard(latinSquare);
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 4 && std::string_view(argv[1]).compare(LatinSquare::LATIN_SQUARES_TEMPLATE_FILE) == 0) {
        const auto [size, numbers] = LatinSquare::convert(argv[2]);
        const std::string filename = argv[3];

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()
            && std::string_view(filename).compare("") != 0) {
            const auto start = std::chrono::steady_clock::now();
            const auto latinSquare = latinSquareGenerator.random(size, numbers);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            LatinSquare::printFile(latinSquare, filename);
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3 && std::string_view(argv[1]).compare(LatinSquare::LATIN_SQUARES_TEMPLATE_COUNT) == 0) {
        const auto [size, numbers] = LatinSquare::convert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto latinSquare = LatinSquare::LatinSquare(size, numbers);
            const auto start = std::chrono::steady_clock::now();
            const auto count = latinSquareGenerator.count(latinSquare);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            std::cout.write(LatinSquare::LATIN_SQUARES.data(), LatinSquare::LATIN_SQUARES.length());
            std::cout << count << std::endl;
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 4 && std::string_view(argv[1]).compare(LatinSquare::SYMMETRIC_LATIN_SQUARES_RANDOM) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown) {
            const auto start = std::chrono::steady_clock::now();
            const auto symmetricLatinSquare = latinSquareGenerator.symmetricRandom(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            LatinSquare::printBoard(symmetricLatinSquare);
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 5 && std::string_view(argv[1]).compare(LatinSquare::SYMMETRIC_LATIN_SQUARES_FILE) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);
        const std::string filename = argv[4];

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown
            && std::string_view(filename).compare("") != 0) {
            const auto start = std::chrono::steady_clock::now();
            const auto symmetricLatinSquare = latinSquareGenerator.symmetricRandom(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            LatinSquare::printTriangularFile(symmetricLatinSquare, filename);
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 4 && std::string_view(argv[1]).compare(LatinSquare::SYMMETRIC_LATIN_SQUARES_COUNT) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown) {
            const auto start = std::chrono::steady_clock::now();
            const auto count = latinSquareGenerator.symmetricCount(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            std::cout.write(LatinSquare::LATIN_SQUARES.data(), LatinSquare::LATIN_SQUARES.length());
            std::cout << count << std::endl;
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3
               && std::string_view(argv[1]).compare(LatinSquare::SYMMETRIC_LATIN_SQUARES_TEMPLATE_RANDOM) == 0) {
        const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            const auto start = std::chrono::steady_clock::now();
            const auto symmetricLatinSquare = latinSquareGenerator.symmetricRandom(size, numbers);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            LatinSquare::printBoard(symmetricLatinSquare);
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 4
               && std::string_view(argv[1]).compare(LatinSquare::SYMMETRIC_LATIN_SQUARES_TEMPLATE_FILE) == 0) {
        const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);
        const std::string filename = argv[3];

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()
            && std::string_view(filename).compare("") != 0) {
            const auto start = std::chrono::steady_clock::now();
            const auto symmetricLatinSquare = latinSquareGenerator.symmetricRandom(size, numbers);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            LatinSquare::printTriangularFile(symmetricLatinSquare, filename);
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3
               && std::string_view(argv[1]).compare(LatinSquare::SYMMETRIC_LATIN_SQUARES_TEMPLATE_COUNT) == 0) {
        const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto symmetricLatinSquare = LatinSquare::SymmetricLatinSquare(size, numbers);
            const auto start = std::chrono::steady_clock::now();
            const auto count = latinSquareGenerator.symmetricCount(symmetricLatinSquare);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(LatinSquare::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(LatinSquare::SECONDS);
            std::cout.write(LatinSquare::LATIN_SQUARES.data(), LatinSquare::LATIN_SQUARES.length());
            std::cout << count << std::endl;
            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3 && std::string_view(argv[1]).compare(Transversal::TRANSVERSALS_RANDOM) == 0) {
        const auto [size, numbers] = LatinSquare::convert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto latinSquare = LatinSquare::LatinSquare(size, numbers);

            if (!latinSquare.notFilled()) {
                latinSquare.setRegions();
                const auto start = std::chrono::steady_clock::now();
                auto transversal = transversalGenerator.random(latinSquare);
                const auto stop = std::chrono::steady_clock::now();
                const auto duration = std::chrono::duration<double, std::micro>(stop - start);
                const auto seconds = duration.count() / 1000000.0;
                std::string timeString;
                timeString.append(LatinSquare::TIME);
                timeString.append(std::to_string(seconds));
                timeString.append(LatinSquare::SECONDS);
                Transversal::printBoard(latinSquare, transversal);
                std::cout.write(timeString.c_str(), timeString.size());
                return 0;
            }
        }
    } else if (argc == 3 && std::string_view(argv[1]).compare(Transversal::TRANSVERSALS_COUNT) == 0) {
        const auto [size, numbers] = LatinSquare::convert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto latinSquare = LatinSquare::LatinSquare(size, numbers);

            if (!latinSquare.notFilled()) {
                latinSquare.setRegions();
                const auto start = std::chrono::steady_clock::now();
                const auto count = transversalGenerator.count(latinSquare);
                const auto stop = std::chrono::steady_clock::now();
                const auto duration = std::chrono::duration<double, std::micro>(stop - start);
                const auto seconds = duration.count() / 1000000.0;
                std::string timeString;
                timeString.append(Transversal::TIME);
                timeString.append(std::to_string(seconds));
                timeString.append(Transversal::SECONDS);
                std::cout.write(Transversal::TRANSVERSALS.data(), Transversal::TRANSVERSALS.length());
                std::cout << count << std::endl;
                std::cout.write(timeString.c_str(), timeString.size());
                return 0;
            }
        }
    } else if (argc == 4 && std::string_view(argv[1]).compare(Transversal::TRANSVERSALS_MINMAX) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown) {
            const auto start = std::chrono::steady_clock::now();
            const auto counts = transversalGenerator.minMax(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(Transversal::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(Transversal::SECONDS);
            std::cout.write(Transversal::MIN_TRANSVERSALS.data(), Transversal::MIN_TRANSVERSALS.length());
            std::cout << counts[0].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.length());
            std::cout << counts[0].latinSquaresCounter() << std::endl;

            if (!counts[0].latinSquare().notFilled()) {
                LatinSquare::printBoard(counts[0].latinSquare());
            }

            std::cout.write(Transversal::MAX_TRANSVERSALS.data(), Transversal::MAX_TRANSVERSALS.length());
            std::cout << counts[1].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.length());
            std::cout << counts[1].latinSquaresCounter() << std::endl;

            if (!counts[1].latinSquare().notFilled()) {
                LatinSquare::printBoard(counts[1].latinSquare());
            }

            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3 && std::string_view(argv[1]).compare(Transversal::TRANSVERSALS_TEMPLATE) == 0) {
        const auto [size, numbers] = LatinSquare::convert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto latinSquare = LatinSquare::LatinSquare(size, numbers);
            const auto start = std::chrono::steady_clock::now();
            const auto counts = transversalGenerator.minMax(latinSquare);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(Transversal::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(Transversal::SECONDS);
            std::cout.write(Transversal::MIN_TRANSVERSALS.data(), Transversal::MIN_TRANSVERSALS.length());
            std::cout << counts[0].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.length());
            std::cout << counts[0].latinSquaresCounter() << std::endl;

            if (!counts[0].latinSquare().notFilled()) {
                LatinSquare::printBoard(counts[0].latinSquare());
            }

            std::cout.write(Transversal::MAX_TRANSVERSALS.data(), Transversal::MAX_TRANSVERSALS.length());
            std::cout << counts[1].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.length());
            std::cout << counts[1].latinSquaresCounter() << std::endl;

            if (!counts[1].latinSquare().notFilled()) {
                LatinSquare::printBoard(counts[1].latinSquare());
            }

            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3 && std::string_view(argv[1]).compare(Transversal::SYMMETRIC_TRANSVERSALS_RANDOM) == 0) {
        const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto symmetricLatinSquare = LatinSquare::SymmetricLatinSquare(size, numbers);

            if (!symmetricLatinSquare.notFilled()) {
                symmetricLatinSquare.fillGrid();
                symmetricLatinSquare.setRegions();
                const auto start = std::chrono::steady_clock::now();
                auto transversal = transversalGenerator.symmetricRandom(symmetricLatinSquare);
                const auto stop = std::chrono::steady_clock::now();
                const auto duration = std::chrono::duration<double, std::micro>(stop - start);
                const auto seconds = duration.count() / 1000000.0;
                std::string timeString;
                timeString.append(LatinSquare::TIME);
                timeString.append(std::to_string(seconds));
                timeString.append(LatinSquare::SECONDS);
                Transversal::printBoard(symmetricLatinSquare, transversal);
                std::cout.write(timeString.c_str(), timeString.size());
                return 0;
            }
        }
    } else if (argc == 3 && std::string_view(argv[1]).compare(Transversal::SYMMETRIC_TRANSVERSALS_COUNT) == 0) {
        const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto symmetricLatinSquare = LatinSquare::SymmetricLatinSquare(size, numbers);

            if (!symmetricLatinSquare.notFilled()) {
                symmetricLatinSquare.fillGrid();
                symmetricLatinSquare.setRegions();
                const auto start = std::chrono::steady_clock::now();
                const auto count = transversalGenerator.symmetricCount(symmetricLatinSquare);
                const auto stop = std::chrono::steady_clock::now();
                const auto duration = std::chrono::duration<double, std::micro>(stop - start);
                const auto seconds = duration.count() / 1000000.0;
                std::string timeString;
                timeString.append(Transversal::TIME);
                timeString.append(std::to_string(seconds));
                timeString.append(Transversal::SECONDS);
                std::cout.write(Transversal::TRANSVERSALS.data(), Transversal::TRANSVERSALS.length());
                std::cout << count << std::endl;
                std::cout.write(timeString.c_str(), timeString.size());
                return 0;
            }
        }
    } else if (argc == 4 && std::string_view(argv[1]).compare(Transversal::SYMMETRIC_TRANSVERSALS_MINMAX) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown) {
            const auto start = std::chrono::steady_clock::now();
            const auto counts = transversalGenerator.symmetricMinMax(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(Transversal::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(Transversal::SECONDS);
            std::cout.write(Transversal::MIN_TRANSVERSALS.data(), Transversal::MIN_TRANSVERSALS.length());
            std::cout << counts[0].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.length());
            std::cout << counts[0].symmetricLatinSquaresCounter() << std::endl;

            if (!counts[0].symmetricLatinSquare().notFilled()) {
                LatinSquare::printBoard(counts[0].symmetricLatinSquare());
            }

            std::cout.write(Transversal::MAX_TRANSVERSALS.data(), Transversal::MAX_TRANSVERSALS.length());
            std::cout << counts[1].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.length());
            std::cout << counts[1].symmetricLatinSquaresCounter() << std::endl;

            if (!counts[1].symmetricLatinSquare().notFilled()) {
                LatinSquare::printBoard(counts[1].symmetricLatinSquare());
            }

            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3 && std::string_view(argv[1]).compare(Transversal::SYMMETRIC_TRANSVERSALS_TEMPLATE) == 0) {
        const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto symmetricLatinSquare = LatinSquare::SymmetricLatinSquare(size, numbers);
            const auto start = std::chrono::steady_clock::now();
            const auto counts = transversalGenerator.symmetricMinMax(symmetricLatinSquare);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(Transversal::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(Transversal::SECONDS);
            std::cout.write(Transversal::MIN_TRANSVERSALS.data(), Transversal::MIN_TRANSVERSALS.length());
            std::cout << counts[0].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.length());
            std::cout << counts[0].symmetricLatinSquaresCounter() << std::endl;

            if (!counts[0].symmetricLatinSquare().notFilled()) {
                LatinSquare::printBoard(counts[0].symmetricLatinSquare());
            }

            std::cout.write(Transversal::MAX_TRANSVERSALS.data(), Transversal::MAX_TRANSVERSALS.length());
            std::cout << counts[1].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.length());
            std::cout << counts[1].symmetricLatinSquaresCounter() << std::endl;

            if (!counts[1].symmetricLatinSquare().notFilled()) {
                LatinSquare::printBoard(counts[1].symmetricLatinSquare());
            }

            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    // } else if (argc == 3
    //          && std::string_view(argv[1]).compare(Transversal::SYMMETRIC_TRIANGULAR_TRANSVERSALS_RANDOM) == 0) {
    //     const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);

    //     if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
    //         auto symmetricLatinSquare = LatinSquare::SymmetricLatinSquare(size, numbers);

    //         if (!symmetricLatinSquare.notFilled()) {
    //             symmetricLatinSquare.setNumberRegions();
    //             const auto start = std::chrono::steady_clock::now();
    //             auto transversal = transversalGenerator.symmetricTriangularRandom(symmetricLatinSquare);
    //             const auto stop = std::chrono::steady_clock::now();
    //             const auto duration = std::chrono::duration<double, std::micro>(stop - start);
    //             const auto seconds = duration.count() / 1000000.0;
    //             std::string timeString;
    //             timeString.append(LatinSquare::TIME);
    //             timeString.append(std::to_string(seconds));
    //             timeString.append(LatinSquare::SECONDS);
    //             Transversal::printTriangularBoard(symmetricLatinSquare, transversal);
    //             std::cout.write(timeString.c_str(), timeString.size());
    //             return 0;
    //         }
    //     }
    } else if (argc == 3
               && std::string_view(argv[1]).compare(Transversal::SYMMETRIC_TRIANGULAR_TRANSVERSALS_COUNT) == 0) {
        const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto symmetricLatinSquare = LatinSquare::SymmetricLatinSquare(size, numbers);

            if (!symmetricLatinSquare.notFilled()) {
                symmetricLatinSquare.setNumberRegions();
                const auto start = std::chrono::steady_clock::now();
                const auto count = transversalGenerator.symmetricTriangularCount(symmetricLatinSquare);
                const auto stop = std::chrono::steady_clock::now();
                const auto duration = std::chrono::duration<double, std::micro>(stop - start);
                const auto seconds = duration.count() / 1000000.0;
                std::string timeString;
                timeString.append(Transversal::TIME);
                timeString.append(std::to_string(seconds));
                timeString.append(Transversal::SECONDS);
                std::cout.write(Transversal::TRANSVERSALS.data(), Transversal::TRANSVERSALS.length());
                std::cout << count << std::endl;
                std::cout.write(timeString.c_str(), timeString.size());
                return 0;
            }
        }
    } else if (argc == 4
               && std::string_view(argv[1]).compare(Transversal::SYMMETRIC_TRIANGULAR_TRANSVERSALS_MINMAX) == 0) {
        const uint_fast8_t size = std::strtoul(argv[2], nullptr, 10);
        const LatinSquare::Type type = LatinSquare::getType(argv[3]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && type != LatinSquare::Type::Unknown) {
            const auto start = std::chrono::steady_clock::now();
            const auto counts = transversalGenerator.symmetricTriangularMinMax(size, type);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(Transversal::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(Transversal::SECONDS);
            std::cout.write(Transversal::MIN_TRANSVERSALS.data(), Transversal::MIN_TRANSVERSALS.length());
            std::cout << counts[0].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.length());
            std::cout << counts[0].symmetricLatinSquaresCounter() << std::endl;

            if (!counts[0].symmetricLatinSquare().notFilled()) {
                LatinSquare::printTriangularBoard(counts[0].symmetricLatinSquare());
            }

            std::cout.write(Transversal::MAX_TRANSVERSALS.data(), Transversal::MAX_TRANSVERSALS.length());
            std::cout << counts[1].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.length());
            std::cout << counts[1].symmetricLatinSquaresCounter() << std::endl;

            if (!counts[1].symmetricLatinSquare().notFilled()) {
                LatinSquare::printTriangularBoard(counts[1].symmetricLatinSquare());
            }

            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    } else if (argc == 3
               && std::string_view(argv[1]).compare(Transversal::SYMMETRIC_TRIANGULAR_TRANSVERSALS_TEMPLATE) == 0) {
        const auto [size, numbers] = LatinSquare::symmetricConvert(argv[2]);

        if (size > 0 && size <= LatinSquare::MAX_SIZE && numbers.size()) {
            auto symmetricLatinSquare = LatinSquare::SymmetricLatinSquare(size, numbers);
            const auto start = std::chrono::steady_clock::now();
            const auto counts = transversalGenerator.symmetricTriangularMinMax(symmetricLatinSquare);
            const auto stop = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration<double, std::micro>(stop - start);
            const auto seconds = duration.count() / 1000000.0;
            std::string timeString;
            timeString.append(Transversal::TIME);
            timeString.append(std::to_string(seconds));
            timeString.append(Transversal::SECONDS);
            std::cout.write(Transversal::MIN_TRANSVERSALS.data(), Transversal::MIN_TRANSVERSALS.length());
            std::cout << counts[0].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MIN_TRANSVERSALS.length());
            std::cout << counts[0].symmetricLatinSquaresCounter() << std::endl;

            if (!counts[0].symmetricLatinSquare().notFilled()) {
                LatinSquare::printTriangularBoard(counts[0].symmetricLatinSquare());
            }

            std::cout.write(Transversal::MAX_TRANSVERSALS.data(), Transversal::MAX_TRANSVERSALS.length());
            std::cout << counts[1].counter() << std::endl;
            std::cout.write(Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.data(),
                Transversal::LATIN_SQUARES_MAX_TRANSVERSALS.length());
            std::cout << counts[1].symmetricLatinSquaresCounter() << std::endl;

            if (!counts[1].symmetricLatinSquare().notFilled()) {
                LatinSquare::printTriangularBoard(counts[1].symmetricLatinSquare());
            }

            std::cout.write(timeString.c_str(), timeString.size());
            return 0;
        }
    }

    std::cout.write(LatinSquare::USAGE.data(), LatinSquare::USAGE.length());
    return 0;
}
