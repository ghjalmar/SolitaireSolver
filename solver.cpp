#include <iostream>
#include <iterator>
#include <unordered_set>

#include "solver.h"
#include "random_picks.h"

#include <iostream>

namespace solitaire
{
    Solutions::Solutions() : solutions_{}
    {
        solutions_.reserve(100'000);
    }

    void Solutions::appendSolution(solitaire::Solution const& solution)
    {
        solutions_.push_back(solution);
    }

    void Solutions::appendMove(Move const& move)
    {
        if (solutions_.empty())
        {
            solutions_.emplace_back();
            solutions_.back().reserve(32);
        }
        solutions_.back().push_back(move);
    }

    bool Solutions::contains(Solution const& solution) const
    {
        return std::find(solutions_.begin(), solutions_.end(), solution) != solutions_.end();
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

    std::size_t Solutions::size() const
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

    bool SolveBoardRandomly(solitaire::Board board, Solution &solution, std::uint32_t& randomSelectionsMade, std::uint32_t const& maxRandomSelections)
    {
        static std::uint32_t iter{0};

        static RandomIntBetweenAAndB randomIntBetween1And100{1, 100};
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
                return true;
            }
            return false;
        }

        if (availableMoves.size() > 1 && solution.size() < 16 && randomSelectionsMade < maxRandomSelections)
        {
            RandomMovePicker randomMovePicker{availableMoves};
            ++randomSelectionsMade;
            std::cout << "Choosing a random move for the " << std::to_string(randomSelectionsMade) << " time." << std::endl;
            availableMoves = randomMovePicker.pickRandomMoves();
        }
        for (auto const& move : availableMoves)
        {
            auto const boardIteration = board.applyMove(move);
            solution.push_back(move);
            if (!SolveBoardRandomly(boardIteration, solution, randomSelectionsMade, maxRandomSelections))
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
