#include <algorithm>
#include <iostream>
#include <string>

#include "board.h"

std::string PrintAvailableMoves(solitaire::AvailableMoves const& moves)
{
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

namespace solitaire
{
    bool SolveBoard(solitaire::Board board, std::vector<solitaire::Move>& solution)
    {
        static int iter;
        ++iter;

        auto availableMoves = board.getAvailableMoves();
        if (availableMoves.empty())
        {
            if (board.isSolved())
            {
                return true;
            }
            std::cout << "No available moves left on the table, but no solution found. iter: " << iter <<
                " Length of solutions: " << solution.size() << std::endl;
            return false;
        }
        std::cout << PrintAvailableMoves(availableMoves) << std::endl;

        bool solutionFound{false};
        for (auto const& move : availableMoves)
        {
            auto const boardIteration = board.applyMove(move);
            solution.push_back(move);
            std::cout << boardIteration.getBoardState() << "\n" << std::endl;
//            if (solution.size() > 31)
//            {
//                return true;
//            }
            if (!SolveBoard(boardIteration, solution))
            {
                std::cout << "ERASING FROM THE END OF THE SOLUTION VECTOR!!" << std::endl;
                std::cout << boardIteration.getBoardState() << std::endl;
                solution.pop_back();
            }
            else
            {
                solutionFound = true;
                break;
            }
        }
        return solutionFound;
    }
}

int main()
{
    std::cout << "Halló heimur!" << std::endl;
    auto const bord = solitaire::Board();
    std::cout << "Upprunaleg staða á borði: " << std::endl;
    std::cout << bord.getBoardState() << std::endl;
    std::cout << PrintAvailableMoves(bord.getAvailableMoves()) << std::endl;
    std::vector<solitaire::Move> solution{};
    if (solitaire::SolveBoard(bord, solution))
    {
        std::cout << "Fann lausn!" << std::endl;
    }
    else
    {
        std::cout << "Fann enga lausn." << std::endl;
    }
    return 0;
}
