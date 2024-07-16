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

void FirstSolution()
{
    auto const tick = std::chrono::system_clock::now();
    auto const firstMove{solitaire::Move{{1, 3},{3, 3}}};
    auto const board = solitaire::Board().applyMove(firstMove);
    solitaire::Solution firstSolution{};
    firstSolution.push_back(firstMove);
    solitaire::SolveBoardOnce(board, firstSolution);
    auto const tock = std::chrono::system_clock::now();
    std::cout << "The first solution found: \n" << common::PrintAvailableMoves(firstSolution) << std::endl;
    std::cout << "Found the solution in  " << common::TimeDiffAsUs(tock, tick)/1e6 << " seconds." << std::endl;
}

void RandomSolution()
{
    std::uint32_t randomSelectionsMade{0};
    std::uint32_t const maxRandomSelections = solitaire::RandomIntBetweenAAndB{3, 10}();
    auto const tick = std::chrono::system_clock::now();
    solitaire::Solution solution{};
    solitaire::SolveBoardRandomly(solitaire::Board{}, solution, randomSelectionsMade, maxRandomSelections);
    auto const tock = std::chrono::system_clock::now();
    auto time = common::TimeDiffAsUs(tock, tick)/1e6f;
    std::cout << "Found a random solution: \n" << common::PrintAvailableMoves(solution) << std::endl;
    std::cout << "Found the random solution in " << time << " seconds." << std::endl;
}

void BruteforceAllSolutions()
{
    auto const tick = std::chrono::system_clock::now();
    auto const firstMove{solitaire::Move{{1, 3},
                                         {3, 3}}};
    auto const board = solitaire::Board().applyMove(firstMove);
    auto const possibleMoves = board.getAvailableMoves();

    std::vector<std::thread> solverThreads;
    solverThreads.reserve(possibleMoves.size());
    std::vector<std::pair<solitaire::Solutions, solitaire::Board>> parallelSolutions{};
    parallelSolutions.reserve(possibleMoves.size());

    std::cout << "Possible moves after having applied the first move: " << possibleMoves.size() << std::endl;
    for (auto const& move : possibleMoves)
    {
        auto const parallelBoard = board.applyMove(move);
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
    std::cout << "The first solution: \n" << common::PrintAvailableMoves(std::get<solitaire::Solutions>(parallelSolutions.front()).front()) << std::endl;
    std::cout << "The last solution: \n" << common::PrintAvailableMoves(std::get<solitaire::Solutions>(parallelSolutions.back()).back()) << std::endl;
    std::cout << "It took a total of  " << common::TimeDiffAsUs(tock, tick)/(60.0*1e6) << " hours to find all the solutions." << std::endl;
}


int main(int argc, char* argv[])
{
    const auto commandlineOptionFirstSolution = std::string{"first"};
    const auto commandlineOptionRandom = std::string{"random"};
    const auto commandlineOptionBruteforce = std::string{"bruteforce"};

    std::cout << "The game's starting position:" << std::endl;
    std::cout << solitaire::Board().getBoardState() << std::endl;
    std::cout << "Available moves from the starting position:" << std::endl;
    std::cout << common::PrintAvailableMoves(solitaire::Board().getAvailableMoves()) << std::endl;

    if (argc == 2)
    {
        auto const commandlineOption = std::string{argv[1]};
        if (commandlineOption == commandlineOptionRandom)
        {
            RandomSolution();
        }
        else if (commandlineOption == commandlineOptionBruteforce)
        {
            BruteforceAllSolutions();
        }
        else if (commandlineOption == "first")
        {
            FirstSolution();
        }
        else
        {
            std::cout << "Unknown commandline option: '" << commandlineOption << "'. Usage: " <<
                argv[0] << " <random|bruteforce|first>" << std::endl;;
        }
    }
    else
    {
        FirstSolution();
    }
    return 0;
}
