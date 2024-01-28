#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include "board.h"
#include "solver.h"

std::string PrintAvailableMoves(solitaire::AvailableMoves const& moves)
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

std::string PrintSolutionMoves(std::vector<solitaire::Move> const& moves)
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
    std::cout << "Halló heimur!" << std::endl;
    std::cout << "Upprunaleg staða á borði: " << std::endl;
    std::cout << solitaire::Board().getBoardState() << std::endl;
    std::cout << PrintAvailableMoves(solitaire::Board().getAvailableMoves()) << std::endl;

    // First solution
    if (false)
    {
        auto const tick = std::chrono::system_clock::now();
        solitaire::Solution solution{};
        auto const firstMove{solitaire::Move{{1, 3},
                                             {3, 3}}};
        auto const bord = solitaire::Board().applyMove(firstMove);
        solution.push_back(firstMove);
        solitaire::SolveBoardOnce(bord, solution);
        auto const tock = std::chrono::system_clock::now();
        std::cout << "Fyrsta lausn: \n" << PrintSolutionMoves(solution) << std::endl;
        std::cout << "Lausnin fannst á " << (tock - tick).count() / (60e9) << " mínútum." << std::endl;
    }

    // Brute force all solutions
    {
        auto const tick = std::chrono::system_clock::now();
        auto const firstMove{solitaire::Move{{1, 3},
                                             {3, 3}}};
        auto const bord = solitaire::Board().applyMove(firstMove);
        auto const possibleMoves = bord.getAvailableMoves();

        std::vector<std::jthread> solverThreads;
        solverThreads.reserve(possibleMoves.size());
        std::vector<std::pair<solitaire::Solutions, solitaire::Board>> parallelSolutions{};
        parallelSolutions.reserve(possibleMoves.size());

        std::cout << "Mogulegir leikir: " << possibleMoves.size() << std::endl;
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
                    [&parallelSolution](std::stop_token) {
                        solitaire::SolveBoard(std::get<solitaire::Board>(parallelSolution),
                                              std::get<solitaire::Solutions>(parallelSolution));
                    }
            );
            std::cout << "Thradur i farinn af stað!" << std::endl;
        }
//        std::cout << "Thraedir farnir af stad! Bid i 60 sek." << std::endl;
//        std::this_thread::sleep_for(std::chrono::seconds{60});
        for (auto& thread : solverThreads)
        {
            thread.join();
        }
        auto const tock = std::chrono::system_clock::now();
        std::cout << "Fyrsta lausnin: \n" << PrintSolutionMoves(std::get<solitaire::Solutions>(parallelSolutions.front()).front()) << std::endl;
        std::cout << "Seinasta lausnin: \n" << PrintSolutionMoves(std::get<solitaire::Solutions>(parallelSolutions.back()).back()) << std::endl;
        std::cout << "Lausnirnar fundust á " << (tock - tick).count() / (60e9) << " mínútum." << std::endl;
    }
    return 0;
}
