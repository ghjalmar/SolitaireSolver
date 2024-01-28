#include <iostream>
#include <string>

#include "board.h"

namespace test
{
    std::string expectedInitialBoardState
    {
        "  XXX  \n"
        "  XXX  \n"
        "XXXXXXX\n"
        "XXXOXXX\n"
        "XXXXXXX\n"
        "  XXX  \n"
        "  XXX  \n"
    };

    // Test the board constructor
    int testInitialBoardState()
    {
        if (expectedInitialBoardState != solitaire::Board().getBoardState())
        {
            std::cout << "testInitialBoardState FAILED!" << std::endl;
            std::cout << expectedInitialBoardState << std::endl;
            std::cout << solitaire::Board().getBoardState() << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cout << "testInitialBoardState PASSED!" << std::endl;
            return EXIT_SUCCESS;
        }
    };
}

int main()
{
    auto success = test::testInitialBoardState();
    return EXIT_SUCCESS;
}
