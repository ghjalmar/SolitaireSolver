#include <chrono>
#include <ratio>

#include "common.h"

namespace common
{
    float TimeDiffAsUs(std::chrono::system_clock::time_point const tock, std::chrono::system_clock::time_point const tick)
    {
        if (std::chrono::system_clock::duration::period::den == 1e9)
        {
            return static_cast<float>((tock - tick).count()/1000.0f);
        }

        return static_cast<float>((tock - tick).count());
    }
}
