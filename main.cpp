#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <numeric>

#include "board.h"
#include "common.h"
#include "random_picks.h"
#include "solver.h"

template<typename Iterable>
std::string PrintAvailableMoves(Iterable const& moves)
{
    if (moves.empty()) return "";

    std::string printableMoves{};
    std::for_each(moves.cbegin(), moves.cend(),
                  [&printableMoves](solitaire::Move const& move)
                    {
                        auto const& [row1, col1] = move.first;
                        auto const& [row2, col2] = move.second;
                        printableMoves.append(std::to_string(row1) + std::to_string(col1));
                        printableMoves.append(", ");
                        printableMoves.append(std::to_string(row2) + std::to_string(col2));
                        printableMoves.append("\n");
                    });
    return printableMoves.erase(printableMoves.size()-1);
}

int main()
{
    std::cout << "The game's starting position: " << std::endl;
    std::cout << solitaire::Board().getBoardState() << std::endl;
    std::cout << PrintAvailableMoves(solitaire::Board().getAvailableMoves()) << std::endl;

    auto const firstMove{solitaire::Move{{1, 3},{3, 3}}};
    auto const board = solitaire::Board().applyMove(firstMove);

     // Find the first solution
     {
         auto const tick = std::chrono::system_clock::now();
         solitaire::Solution solution{};
         solution.push_back(firstMove);
         solitaire::SolveBoardOnce(board, solution);
         auto const tock = std::chrono::system_clock::now();
         std::cout << "The first solution found: \n" << PrintAvailableMoves(solution) << std::endl;
         std::cout << "Found the solution in  " << common::TimeDiffAsUs(tock, tick)/1e6 << " seconds." << std::endl;
     }

    // Find a random solution
    solitaire::Solutions randomSolutions{};
    float minTime{std::numeric_limits<float>::max()};
    float maxTime{0.0f};
    float totalTime{0.0f};
    std::uint8_t iterations{0};
    while (randomSolutions.size() < 10)
    {
        std::uint32_t randomSelectionsMade{0};
        std::uint32_t const maxRandomSelections = solitaire::RandomIntBetweenAAndB{3, 10}();
        auto const tick = std::chrono::system_clock::now();
        solitaire::Solution solution{firstMove};
        solitaire::SolveBoardRandomly(board, solution, randomSelectionsMade, maxRandomSelections);
        auto const tock = std::chrono::system_clock::now();
        if (!randomSolutions.contains(solution))
        {
            randomSolutions.appendSolution(solution);
            std::cout << "A new solution found. Total unique solutions found: " << randomSolutions.size() << std::endl;
            std::cout << "The solution: \n" << PrintAvailableMoves(solution) << std::endl;
        }
        auto time = common::TimeDiffAsUs(tock, tick)/1e6f;
        std::cout << "Found a random solution in " << time << " seconds." << std::endl;
        if (time < minTime) minTime = time;
        if (time > maxTime) maxTime = time;
        totalTime += time;
        ++iterations;
    }
    std::cout << "The fastest solution took " << minTime << " seconds to find." << std::endl;
    std::cout << "The slowest solution took " << maxTime << " seconds to find." << std::endl;
    std::cout << "Found the 10 unique solutions in " << iterations << " iterations." << std::endl;

//    // Brute force to find all solutions
//    {
//        auto const tick = std::chrono::system_clock::now();
//        auto const firstMove{solitaire::Move{{1, 3},
//                                             {3, 3}}};
//        auto const board = solitaire::Board().applyMove(firstMove);
//        auto const possibleMoves = board.getAvailableMoves();
//
//        std::vector<std::thread> solverThreads;
//        solverThreads.reserve(possibleMoves.size());
//        std::vector<std::pair<solitaire::Solutions, solitaire::Board>> parallelSolutions{};
//        parallelSolutions.reserve(possibleMoves.size());
//
//        std::cout << "Possible moves after having applied the first move: " << possibleMoves.size() << std::endl;
//        for (auto const& move : possibleMoves)
//        {
//            auto const parallelBoard = board.applyMove(move);
//            parallelSolutions.push_back(std::pair{solitaire::Solutions{}, parallelBoard});
//            std::get<solitaire::Solutions>(parallelSolutions.back()).appendMove(firstMove);
//            std::get<solitaire::Solutions>(parallelSolutions.back()).appendMove(move);
//        }
//        for (auto& parallelSolution : parallelSolutions)
//        {
//            solverThreads.emplace_back(
//                    [&parallelSolution]()
//                    {
//                        solitaire::SolveBoard(std::get<solitaire::Board>(parallelSolution),
//                                              std::get<solitaire::Solutions>(parallelSolution));
//                    }
//            );
//        }
//        std::cout << "Parallel threads launched." << std::endl;
//        for (auto& thread : solverThreads)
//        {
//            thread.join();
//        }
//        auto const tock = std::chrono::system_clock::now();
//        std::size_t totalSolutions = std::accumulate(parallelSolutions.cbegin(), parallelSolutions.cend(), 0,
//                      [](std::size_t sum, auto const solutionsSubset)
//                      {
//                        return sum + std::get<solitaire::Solutions>(solutionsSubset).size();
//                      });
//        std::cout << "Finished search for solutions. Found a total of " << totalSolutions << " solutions." << std::endl;
//        std::cout << "The first solution: \n" << PrintAvailableMoves(std::get<solitaire::Solutions>(parallelSolutions.front()).front()) << std::endl;
//        std::cout << "The last solution: \n" << PrintAvailableMoves(std::get<solitaire::Solutions>(parallelSolutions.back()).back()) << std::endl;
//        std::cout << "I took a total of  " << common::TimeDiffAsUs(tock, tick)/(60.0*1e6) << " hours to find all the solutions." << std::endl;
//    }
    return 0;
}
