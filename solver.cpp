#include <iostream>

#include "solver.h"

namespace solitaire
{
    Solutions::Solutions() : solutions_{1, Solution{}}
    {
        solutions_.reserve(100'000);
        solutions_.back().reserve(32);
    }

    void Solutions::appendMove(Move move)
    {
        solutions_.back().push_back(move);
    }

    void Solutions::popLastMove()
    {
        solutions_.back().pop_back();
    }

    void Solutions::copyCurrentSolution()
    {
        solutions_.push_back(solutions_.back());
        solutions_.back().reserve(32);
    }

    std::size_t Solutions::size()
    {
        return solutions_.size();
    }

    Solution Solutions::front()
    {
        return solutions_.front();
    }

    Solution Solutions::back()
    {
        return solutions_.back();
    }

    bool SolveBoardOnce(solitaire::Board board, Solution& solution)
    {
        static std::uint32_t iter;
        bool solutionFound{false};

        auto availableMoves = board.getAvailableMoves();
        if (availableMoves.empty())
        {
            ++iter;
            if ((iter % 1'000'000) == 0)
            {
                std::cout << iter/1'000'000 << " million iterations." << std::endl;
            }
            if (board.isSolved())
            {
                std::cout << "Found a solution!" << std::endl;
                solutionFound = true;
            }
        }

        for (auto const& move : availableMoves)
        {
            auto const boardIteration = board.applyMove(move);
            solution.push_back(move);
            if (!SolveBoardOnce(boardIteration, solution))
            {
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

    void SolveBoard(solitaire::Board board, Solutions& solutions)
    {
        auto const availableMoves = board.getAvailableMoves();

        if (availableMoves.empty() && board.isSolved())
        {
            std::cout << "Found solution nbr. " << solutions.size() << "." << std::endl;
            solutions.copyCurrentSolution();
        }

        for (auto const& move : availableMoves)
        {
            auto const boardIteration = board.applyMove(move);
            solutions.appendMove(move);
            SolveBoard(boardIteration, solutions);
            solutions.popLastMove();
        }
    }
}
