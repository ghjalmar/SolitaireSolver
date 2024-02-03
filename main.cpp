#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <numeric>

#include "board.h"
#include "common.h"
#include "solver.h"

template<typename Iterable>
std::string PrintAvailableMoves(Iterable const& moves)
{
    if (moves.empty()) return "";

    std::string printableMoves{};
    std::for_each(moves.cbegin(), moves.cend(),
                  [&printableMoves](solitaire::Move move)
                    {
                        auto [row1, col1] = move.first;
                        auto [row2, col2] = move.second;
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

    // Find the first solution
    {
        auto const tick = std::chrono::system_clock::now();
        solitaire::Solution solution{};
        auto const firstMove{solitaire::Move{{1, 3},
                                             {3, 3}}};
        auto const bord = solitaire::Board().applyMove(firstMove);
        solution.push_back(firstMove);
        solitaire::SolveBoardOnce(bord, solution);
        auto const tock = std::chrono::system_clock::now();
        std::cout << "The first solution found: \n" << PrintAvailableMoves(solution) << std::endl;
        std::cout << "Found the solution in  " << common::TimeDiffAsUs(tock, tick)/1e6 << " seconds." << std::endl;
    }

    // Brute force to find all solutions
    {
        auto const tick = std::chrono::system_clock::now();
        auto const firstMove{solitaire::Move{{1, 3},
                                             {3, 3}}};
        auto const bord = solitaire::Board().applyMove(firstMove);
        auto const possibleMoves = bord.getAvailableMoves();

        std::vector<std::thread> solverThreads;
        solverThreads.reserve(possibleMoves.size());
        std::vector<std::pair<solitaire::Solutions, solitaire::Board>> parallelSolutions{};
        parallelSolutions.reserve(possibleMoves.size());

        std::cout << "Possible moves after having applied the first move: " << possibleMoves.size() << std::endl;
        for (auto const& move : possibleMoves)
        {
            auto const parallelBoard = bord.applyMove(move);
            parallelSolutions.push_back(std::pair{solitaire::Solutions{}, parallelBoard});
            std::get<solitaire::Solutions>(parallelSolutions.back()).appendMove(firstMove);
            std::get<solitaire::Solutions>(parallelSolutions.back()).appendMove(move);
        }
        for (auto& parallelSolution : parallelSolutions)
        {
            solverThreads.emplace_back(
                    [&parallelSolution]()
                    {
                        solitaire::SolveBoard(std::get<solitaire::Board>(parallelSolution),
                                              std::get<solitaire::Solutions>(parallelSolution));
                    }
            );
        }
        std::cout << "Parallel threads launched." << std::endl;
        for (auto& thread : solverThreads)
        {
            thread.join();
        }
        auto const tock = std::chrono::system_clock::now();
        std::size_t totalSolutions = std::accumulate(parallelSolutions.cbegin(), parallelSolutions.cend(), 0,
                      [](std::size_t sum, auto const solutionsSubset)
                      {
                        return sum + std::get<solitaire::Solutions>(solutionsSubset).size();
                      });
        std::cout << "Finished search for solutions. Found a total of " << totalSolutions << " solutions." << std::endl;
        std::cout << "The first solution: \n" << PrintAvailableMoves(std::get<solitaire::Solutions>(parallelSolutions.front()).front()) << std::endl;
        std::cout << "The last solution: \n" << PrintAvailableMoves(std::get<solitaire::Solutions>(parallelSolutions.back()).back()) << std::endl;
        std::cout << "I took a total of  " << common::TimeDiffAsUs(tock, tick)/(60.0*1e6) << " hours to find all the solutions." << std::endl;
    }
    return 0;
}
