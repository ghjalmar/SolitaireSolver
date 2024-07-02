#include <iostream>
#include <iterator>
#include <random>
#include <unordered_set>

#include "solver.h"


#include <iostream>

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

    class RandomMovePicker
    {
    public:
        RandomMovePicker(solitaire::AvailableMoves const originalAvailableMoves) : randomGeneratingEngine_{randomDevice_()}, randomGenerator_{0, static_cast<int>(originalAvailableMoves.size() - 1)}, remainingMoves_{originalAvailableMoves}
        {}

        solitaire::Move randomMove()
        {
            if (remainingMoves_.empty()) return Move{};

            auto it = remainingMoves_.begin();
            std::advance(it, randomGenerator_(randomGeneratingEngine_));
            Move moveToPlay{*it};
            remainingMoves_.erase(it);
            randomGenerator_ = std::uniform_int_distribution<int>{0, static_cast<int>(remainingMoves() - 1)};
            return moveToPlay;
        }


        std::size_t remainingMoves() const
        {
            return remainingMoves_.size();
        }

    private:
        std::random_device randomDevice_{};
        std::mt19937 randomGeneratingEngine_;
        solitaire::AvailableMoves remainingMoves_;
        std::uniform_int_distribution<int> randomGenerator_;
    };

    bool SolveBoardRandomly(solitaire::Board board, Solution& solution)
    {
        bool solutionFound{false};
        auto const availableMoves = board.getAvailableMoves();

        if (availableMoves.empty() && board.isSolved())
        {
            return true;
        }

        RandomMovePicker randomMovePicker{availableMoves};
        while(randomMovePicker.remainingMoves() != 0)
        {
            auto const move = randomMovePicker.randomMove();
            // std::cout << "retrieved a random move from random move picker" << std::endl;
            auto const boardIteration = board.applyMove(move);
            solution.push_back(move);
            if (!SolveBoardRandomly(boardIteration, solution))
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
