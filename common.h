#ifndef SOLITAIRE_HACK_COMMON_H
#define SOLITAIRE_HACK_COMMON_H

#include <chrono>

#include <iostream>

namespace common
{
    float TimeDiffAsUs(std::chrono::system_clock::time_point const tock, std::chrono::system_clock::time_point const tick);
}

#endif /* SOLITAIRE_HACK_COMMON_H */
