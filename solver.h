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

        void appendMove(Move move);
        void copyCurrentSolution();
        void popLastMove();
        Solution back();
        Solution front();
        std::size_t size();

    private:
        std::vector<Solution> solutions_;
    };

    bool SolveBoardOnce(solitaire::Board board, Solution& solution);
    void SolveBoard(solitaire::Board board, Solutions& solutions);
}

#endif //SOLITAIREHACK_SOLVER_H
