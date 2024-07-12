#ifndef SOLITAIREHACK_SOLVER_H
#define SOLITAIREHACK_SOLVER_H

#include <vector>

#include "board.h"

namespace solitaire
{
    using Solution = std::vector<solitaire::Move>;

    class Solutions
    {
    public:
        Solutions();
        ~Solutions() = default;

        void appendSolution(Solution const &solution);
        void appendMove(Move const &move);
        void copyCurrentSolution();
        void popLastMove();
        Solution back();
        bool contains(Solution const &solution) const;
        Solution front();
        std::size_t size() const;

    private:
        std::vector<Solution> solutions_;
    };

    bool SolveBoardOnce(solitaire::Board board, Solution &solution);

    void SolveBoard(solitaire::Board board, Solutions &solutions);

    bool SolveBoardRandomly(solitaire::Board board, Solution &solution, std::uint32_t &randomSelectionsMade,
                            std::uint32_t const &maxRandomSelections);
}

#endif //SOLITAIREHACK_SOLVER_H
