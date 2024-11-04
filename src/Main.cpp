#include "Main.hpp"

#include <random>

#include "LatinSquare/Generator.hpp"
#include "LatinSquare/Utils.hpp"
#include "Transversal/Generator.hpp"
#include "Transversal/Utils.hpp"



#include <chrono>
#include <iostream>


void setup() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

int main() {
    setup();
    std::random_device randomDevice;
    std::mt19937 mersenneTwister(randomDevice());
    auto latinSquareGenerator = LatinSquare::Generator();

    // ---
    auto start = std::chrono::high_resolution_clock::now();
    // ---

    auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(
        10, LatinSquare::Type::Reduced /* 9, LatinSquare::Type::ReducedCyclic */ /* 10, LatinSquare::Type::ReducedDiagonal */, mersenneTwister);

    // ---
    auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration<double, std::micro>(stop - start);
    // std::cout << "Time: " << duration << std::endl;
    // ---

    latinSquare.sortGrid();
    // LatinSquare::printLatinSquareFullIds(latinSquare);
    // LatinSquare::printLatinSquareBoard(latinSquare);

    latinSquare.setRegions();
    auto transversalGenerator = Transversal::Generator();
    auto transversal = transversalGenerator.findRandomTransversal(latinSquare, mersenneTwister);
    // Transversal::printTransversalFullIds(transversal);
    Transversal::printTransversalBoard(latinSquare, transversal);

    // ---
    std::cout << "Time: " << duration << std::endl;
    // ---

    return 0;
}

// TODO:
// - add function to measure time of generating random latin square and random transversal
// - consider using std::move and implementing move for Cell, Region, etc.

// --- TESTING ---
// #include <chrono>
// #include <iostream>

// ---
// auto start = std::chrono::high_resolution_clock::now();
// ---

// ---
// auto stop = std::chrono::high_resolution_clock::now();
// const auto duration = std::chrono::duration<double, std::micro>(stop - start);
// std::cout << "Time: " << duration << std::endl;
// ---
// --- TESTING ---
