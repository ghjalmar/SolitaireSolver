#ifndef SOLITAIRE_HACK_COMMON_H
#define SOLITAIRE_HACK_COMMON_H

#include <chrono>

#include "board.h"

#include <iostream>

namespace common
{
    template<typename Iterable>
    std::string PrintAvailableMoves(Iterable const& moves)
    {
        if (moves.empty()) return "";

        std::string printableMoves{};
        std::for_each(moves.cbegin(), moves.cend(),
                      [&printableMoves](solitaire::Move const& move)
                      {
                          auto const& [row1, col1] = move.first;
                          auto const& [row2, col2] = move.second;
                          printableMoves.append(std::to_string(row1) + std::to_string(col1));
                          printableMoves.append(", ");
                          printableMoves.append(std::to_string(row2) + std::to_string(col2));
                          printableMoves.append("\n");
                      });
        return printableMoves.erase(printableMoves.size()-1);
    }

    float TimeDiffAsUs(std::chrono::system_clock::time_point const tock, std::chrono::system_clock::time_point const tick);
}

#endif /* SOLITAIRE_HACK_COMMON_H */
