#include "Main.hpp"

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
    auto latinSquareGenerator = LatinSquare::Generator();

    // // ---
    // // auto start = std::chrono::high_resolution_clock::now();
    // // ---

    // auto latinSquare = latinSquareGenerator.generateRandomLatinSquare(
    //     10, LatinSquare::Type::Reduced /* 9, LatinSquare::Type::ReducedCyclic */ /* 10, LatinSquare::Type::ReducedDiagonal */, mersenneTwister);

    // // ---
    // // auto stop = std::chrono::high_resolution_clock::now();
    // // const auto duration = std::chrono::duration<double, std::micro>(stop - start);
    // // std::cout << "Time: " << duration << std::endl;
    // // ---

    // latinSquare.sort();
    // // LatinSquare::printLatinSquareFullIds(latinSquare);
    // // LatinSquare::printLatinSquareBoard(latinSquare);

    // latinSquare.setRegions();
    // auto transversalGenerator = Transversal::Generator();
    // auto transversal = transversalGenerator.findRandomTransversal(latinSquare, mersenneTwister);
    // // Transversal::printTransversalFullIds(transversal);
    // Transversal::printTransversalBoard(latinSquare, transversal);

    // ---
    // std::cout << "Time: " << duration << std::endl;
    // ---

    // std::cout << latinSquareGenerator.countAllLatinSquares(2) << std::endl;
    // std::cout << latinSquareGenerator.countAllLatinSquares(3) << std::endl;
    // std::cout << latinSquareGenerator.countAllLatinSquares(4) << std::endl;
    // std::cout << latinSquareGenerator.countAllLatinSquares(5) << std::endl;
    // std::cout << latinSquareGenerator.countAllLatinSquares(6) << std::endl;
    // std::cout << latinSquareGenerator.countAllLatinSquares(7) << std::endl;

    auto latinSquare = latinSquareGenerator.random(13, LatinSquare::Type::ReducedCyclic);
    // auto latinSquare = latinSquareGenerator.random(10, LatinSquare::Type::ReducedDiagonal);
    latinSquare.setRegions();
    auto transversalGenerator = Transversal::Generator();
    // auto transversal = transversalGenerator.random(latinSquare);
    std::cout << "LS generated" << std::endl;

    // ---
    auto start = std::chrono::steady_clock::now();
    // ---

    // LatinSquare::printBoard(latinSquare);

    // Transversal::printIds(latinSquare, transversal);
    // Transversal::printBoard(latinSquare, transversal);


    // auto latinSquare = latinSquareGenerator.random(16, LatinSquare::Type::Reduced);

    const auto count = transversalGenerator.count(latinSquare);
    // const auto count = latinSquareGenerator.count(6);

    // auto latinSquare = latinSquareGenerator.random(20, LatinSquare::Type::Reduced);

    // ---
    auto stop = std::chrono::steady_clock::now();
    std::cout << count << std::endl;
    const auto duration = std::chrono::duration<double, std::micro>(stop - start);
    std::cout << "Time: " << duration.count() / 1000000.0 << " seconds" << std::endl;
    // ---

    // LatinSquare::printBoard(latinSquare);

    return 0;
}
