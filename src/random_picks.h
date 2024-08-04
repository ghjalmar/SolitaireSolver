#ifndef SOLITAIREHACK_RANDOM_PICKS_H
#define SOLITAIREHACK_RANDOM_PICKS_H

#include <random>

#include "board.h"

namespace solitaire
{
    class RandomIntBetweenAAndB
    {
    public:
        explicit RandomIntBetweenAAndB(int minVal, int maxVal);
        int operator()();

    private:
        std::random_device randomDevice_{};
        std::mt19937 randomGeneratingEngine_;
        std::uniform_int_distribution<int> randomGenerator_;

    };

    class RandomMovePicker
    {
    public:
        explicit RandomMovePicker(solitaire::AvailableMoves const& originalAvailableMoves);

        solitaire::Move randomMove();
        solitaire::AvailableMoves pickRandomMoves();
        std::size_t remainingMoves() const;

    private:
        std::random_device randomDevice_{};
        std::mt19937 randomGeneratingEngine_;
        solitaire::AvailableMoves remainingMoves_;
        std::uniform_int_distribution<int> randomGenerator_;
    };
}

#endif //SOLITAIREHACK_RANDOM_PICKS_H
