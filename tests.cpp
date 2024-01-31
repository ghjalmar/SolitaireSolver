#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>

#include "board.h"
#include "solver.h"

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
        "  XXX  "
    };

    // Test the board constructor
    int testInitialBoardState()
    {
        if ((expectedInitialBoardState != solitaire::Board().getBoardState() &&
            solitaire::Board().getNumberOfPegsOnBoard() == 31))
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

    int testIntialAvailableMoves()
    {
        solitaire::AvailableMoves trueMoves{};
        trueMoves.insert({{3, 1}, {3, 3}});
        trueMoves.insert({{3, 5}, {3, 3}});
        trueMoves.insert({{1, 3}, {3, 3}});
        trueMoves.insert({{5, 3}, {3, 3}});

        auto moves = solitaire::Board().getAvailableMoves();
        if (moves == trueMoves)
        {
            std::cout << "testIntialAvailableMoves PASSED!" << std::endl;
            return EXIT_SUCCESS;
        }
        else
        {
            std::cout << "testIntialAvailableMoves FAILED!" << std::endl;
            return EXIT_FAILURE;
        }
    }

    int initialBoardIsNotSolved()
    {
        if (solitaire::Board().isSolved())
        {
            std::cout << "initialBoardIsNotSolved FAILED!" << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "initialBoardIsNotSolved PASSED!" << std::endl;
        return EXIT_SUCCESS;
    }

    int solvedBoardIsSolved()
    {
        solitaire::BoardType board{};
        std::for_each(board.begin(), board.end(),
                       [](solitaire::BoardRow row)
                       {
                            std::for_each(row.begin(), row.end(),
                                [](solitaire::Square square)
                                {
                                    square = solitaire::Square{solitaire::SquareOccupation::empty};
                                }
                            );
                       });
        board[3][3] = solitaire::Square{solitaire::SquareOccupation::occupied};
        auto testBoard = solitaire::Board{board};
        if (testBoard.isSolved())
        {
            std::cout << "solvedBoardIsSolved PASSED!" << std::endl;
            return EXIT_SUCCESS;
        }
        std::cout << "solvedBoardIsSolved FAILED!" << std::endl;
        return EXIT_FAILURE;
    }

    int testApplyMove()
    {
        auto board = solitaire::Board().applyMove({{3, 1}, {3, 3}});
        if (31 != board.getNumberOfPegsOnBoard())
        {
            std::cout << "testApplyMove FAILED!" << std::endl;
            return EXIT_FAILURE;
        }
        if (board.at(3,3).isValidSquare() && board.at(3,3).isOccupied())
        {
            if (board.at(3,2).isValidSquare() && !board.at(3,2).isOccupied())
            {
                if (board.at(3,1).isValidSquare() && !board.at(3,1).isOccupied())
                {
                    std::cout << "testApplyMove PASSED!" << std::endl;
                    return EXIT_SUCCESS;
                }
            }
        }
        std::cout << "testApplyMove FAILED!" << std::endl;
        return EXIT_FAILURE;
    }

    int testSolutions()
    {
        solitaire::Solutions solutions{};
        for (int i{}; i < 32; ++i)
        {
            solutions.appendMove(solitaire::Move{{3, 1}, {3, 3}});
        }
        solutions.copyCurrentSolution();
        solutions.popLastMove();
        if (solutions.size() != 2)
        {
            std::cout << "testSolutions FAILED! solutions.size() != 2). " <<
            "solutions.size() = " << solutions.size() << "." << std::endl;
            return EXIT_FAILURE;
        }
        if (solutions.front().size() != 32)
        {
            std::cout << "testSolutions FAILED! solutions.front().size() != 32 " <<
            "solutions.front().size() = " << solutions.front().size() << "." << std::endl;
            return EXIT_FAILURE;
        }
        if (solutions.back().size() != 31)
        {
            std::cout << "testSolutions FAILED! solutions.back().size() != 31 " <<
            "solutions.back().size() = " << solutions.back().size() << "." << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "testSolutions PASSED!" << std::endl;
        return EXIT_SUCCESS;
    }
}

int main()
{
    auto totalTests{0};
    auto tick = std::chrono::system_clock::now();

    auto testStatus = test::testInitialBoardState();
    ++totalTests;

    testStatus += test::testIntialAvailableMoves();
    ++totalTests;

    testStatus += test::initialBoardIsNotSolved();
    ++totalTests;

    testStatus += test::solvedBoardIsSolved();
    ++totalTests;

    testStatus += test::testApplyMove();
    ++totalTests;

    testStatus += test::testSolutions();
    ++totalTests;

    auto tock = std::chrono::system_clock::now();
    std::cout << "\nRan a total of " << totalTests <<" test(s) in " <<
        (tock-tick).count()/1e3 << " milliseconds." << std::endl;
    if (!testStatus)
    {
        std::cout << "All tests PASSED!" << std::endl;
    }
    else
    {
        std::cout << testStatus << " test(s) FAILED." << std::endl;
    }
    return testStatus;
}
