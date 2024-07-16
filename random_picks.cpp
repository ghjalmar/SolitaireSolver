#include <iostream>

#include "random_picks.h"

namespace solitaire
{
    RandomIntBetweenAAndB::RandomIntBetweenAAndB(int const minVal, int const maxVal) : randomGeneratingEngine_{randomDevice_()}, randomGenerator_{minVal, maxVal}
    {}

    int RandomIntBetweenAAndB::operator()()
    {
        return randomGenerator_(randomGeneratingEngine_);
    }

    RandomMovePicker::RandomMovePicker(solitaire::AvailableMoves const& originalAvailableMoves)
       : randomGeneratingEngine_{randomDevice_()}, randomGenerator_{0, static_cast<int>(originalAvailableMoves.size() - 1)}, remainingMoves_{originalAvailableMoves}
    {}

    solitaire::Move RandomMovePicker::randomMove()
    {
        if (remainingMoves_.empty())
        {
            std::cout << "No more moves to play. Returning empty move instead of a random one." << std::endl;
            return Move{};
        }

        auto it = remainingMoves_.begin();
        std::advance(it, randomGenerator_(randomGeneratingEngine_));
        Move moveToPlay{*it};
        remainingMoves_.erase(it);
        randomGenerator_ = std::uniform_int_distribution<int>{0, static_cast<int>(remainingMoves() - 1)};
        return moveToPlay;
    }

    solitaire::AvailableMoves RandomMovePicker::pickRandomMoves()
    {
        solitaire::AvailableMoves randomMoves{};
        auto movesToPick = RandomIntBetweenAAndB(1, static_cast<int>(remainingMoves()))();
        std::cout << "Picking " << movesToPick << " random move(s)." << std::endl;
        for (int i = 0; i < movesToPick; ++i)
        {
            randomMoves.insert(randomMove());
        }
        return randomMoves;
    }

    std::size_t RandomMovePicker::remainingMoves() const
    {
        return remainingMoves_.size();
    }
}
