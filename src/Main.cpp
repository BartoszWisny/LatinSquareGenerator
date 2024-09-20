#include "Main.hpp"

#include "LatinSquare/Generator.hpp"
#include "LatinSquare/Utils.hpp"
#include "Transversal/Generator.hpp"
#include "Transversal/Utils.hpp"

// --- TESTING ---
// #include "LatinSquare/LatinSquare.hpp"
// #include <chrono>
// #include <iostream>
// --- TESTING ---

int main() {
    auto latinSquareGenerator = LatinSquare::Generator();
    auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(5, true);
    latinSquare.sortGrid();
    // LatinSquare::printLatinSquareFullIds(latinSquare);
    // LatinSquare::printLatinSquareBoard(latinSquare);

    latinSquare.setRegions();
    auto transversalGenerator = Transversal::Generator();
    auto transversal = transversalGenerator.findRandomTransversal(latinSquare);
    // Transversal::printTransversalFullIds(transversal);
    Transversal::printTransversalBoard(latinSquare, transversal);

    return 0;

    // --- TESTING ---

    // std::random_device randomDevice;
    // std::mt19937 mersenneTwister(randomDevice());
    // auto latinSquareGenerator = LatinSquare::Generator();
    // auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(10, true);

    // // ---
    // auto start = std::chrono::high_resolution_clock::now();
    // // ---

    // LatinSquare::printLatinSquareBoard(latinSquare);

    // // ---
    // auto stop = std::chrono::high_resolution_clock::now();
    // const auto duration = std::chrono::duration<double, std::micro>(stop - start);
    // std::cout << "Time: " << duration << std::endl;
    // // ---

    return 0;

    // --- TESTING ---
}

// Improved classes:
// LatinSquare::BacktrackingData, LatinSquare::Cell, LatinSquare::EntropyData, LatinSquare::LatinSquare,
//     LatinSquare::Region, LatinSquare::UpdateData, LatinSquare::Utils
// Transversal::BacktrackingData, Transversal::UpdateData

// TODO:
// - add function to measure time of generating random latin square and random transversal
// - consider moving functions to correct files
// - consider using back_emplacer instead of back_iterator
// - optimize code where possible
// - consider using inline (if possible)
// - consider using std::move
// - consider implementing move for Cell, Region, etc.
// - replace set with vector where possible

// --- TESTING ---
// #include <chrono>
// #include <iostream>
// --- TESTING ---

// ---
// auto start = std::chrono::high_resolution_clock::now();
// ---

// ---
// auto stop = std::chrono::high_resolution_clock::now();
// const auto duration = std::chrono::duration<double, std::micro>(stop - start);
// std::cout << "Time: " << duration << std::endl;
// ---
