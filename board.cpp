#include <algorithm>
#include <string>

#include "board.h"

namespace solitaire
{
    Board::Board() noexcept
    {
        auto shortRow = BoardRow{Square{SquareOccupation::invalid}, Square{SquareOccupation::invalid},
                                 Square{SquareOccupation::occupied}, Square{SquareOccupation::occupied},
                                 Square{SquareOccupation::occupied}, Square{SquareOccupation::invalid},
                                 Square{SquareOccupation::invalid}};
        auto fullRow = BoardRow{Square{SquareOccupation::occupied}, Square{SquareOccupation::occupied},
                                Square{SquareOccupation::occupied}, Square{SquareOccupation::occupied},
                                Square{SquareOccupation::occupied}, Square{SquareOccupation::occupied},
                                Square{SquareOccupation::occupied}};
        auto middleRow = BoardRow{Square{SquareOccupation::occupied}, Square{SquareOccupation::occupied},
                                  Square{SquareOccupation::occupied}, Square{SquareOccupation::empty},
                                  Square{SquareOccupation::occupied}, Square{SquareOccupation::occupied},
                                  Square{SquareOccupation::occupied}};

        board_.at(0) = shortRow;
        board_.at(1) = shortRow;
        board_.at(2) = fullRow;
        board_.at(3) = middleRow;
        board_.at(4) = fullRow;
        board_.at(5) = shortRow;
        board_.at(6) = shortRow;
    }

    Board::Board(BoardType const board) noexcept : board_{board} {};

    Square const& Board::at(Row const row, Col const col) const noexcept
    {
        if (row < 0 || row >= BoardHeight || col < 0 || col >= BoardWidth)
        {
            return invalidSquare_;
        }
        return board_.at(row).at(col);
    }

    AvailableMoves Board::getAvailableMoves() const noexcept
    {
        AvailableMoves availableMoves{};
        for (Row row{0}; row < BoardHeight; ++row)
        {
            for (Col col{0}; col < BoardWidth; ++col)
            {
                if (!this->at(row, col).isValidSquare() || !this->at(row, col).isOccupied()) continue;

                if (this->at(row-1, col).isValidSquare() && this->at(row-1, col).isOccupied())
                {
                    // The square to the left of the current one is occupied, is the left one to that one free?
                    if (this->at(row-2, col).isValidSquare() && !this->at(row-2, col).isOccupied())
                    {
                        availableMoves.insert({{row, col}, {row-2, col}});
                    }
                }
                if (this->at(row+1, col).isValidSquare() && this->at(row+1, col).isOccupied())
                {
                    // The square to the right of the current one is occupied, is the right one to that one free?
                    if (this->at(row+2, col).isValidSquare() && !this->at(row+2, col).isOccupied())
                    {
                        availableMoves.insert({{row, col}, {row+2, col}});
                    }
                }
                if (this->at(row, col-1).isValidSquare() && this->at(row, col-1).isOccupied())
                {
                    // The square above the current one is occupied, is the one above that one free?
                    if (this->at(row, col-2).isValidSquare() && !this->at(row, col-2).isOccupied())
                    {
                        availableMoves.insert({{row, col}, {row, col-2}});
                    }
                }
                if (this->at(row, col+1).isValidSquare() && this->at(row, col+1).isOccupied())
                {
                    // The square below the current one is occupied, is the one below that one free?
                    if (this->at(row, col+2).isValidSquare() && !this->at(row, col+2).isOccupied())
                    {
                        availableMoves.insert({{row, col}, {row, col+2}});
                    }
                }
            }
        }
        return availableMoves;
    }

    std::string Board::getBoardState() const noexcept
    {
        std::string boardState{};
        std::for_each(board_.cbegin(), board_.cend(),
                      [&boardState](BoardRow row)
                      {
                          std::string line{};
                          std::for_each(row.cbegin(), row.cend(),
                             [&line](Square square){line.append(square.occupationAsString());});
                          boardState.append(line.append("\n"));
                      });
        return boardState.erase(boardState.size()-1);
    }

    std::uint8_t Board::getNumberOfPegsOnBoard() const noexcept
    {
        auto totalPegs{0};
        std::for_each(board_.cbegin(), board_.cend(),
                      [&totalPegs](BoardRow row)
                      {
                          std::for_each(row.cbegin(), row.cend(),
                                        [&totalPegs](Square square)
                                        {
                                            if (square.isValidSquare() && square.isOccupied())
                                            {
                                                ++totalPegs;
                                            };
                                        });
                      });
        return totalPegs;
    }

    Board Board::applyMove(solitaire::Move move) const noexcept
    {
        auto const [x0, y0] = move.first;
        auto const [x1, y1] = move.second;
        BoardType newBoard = board_;
        newBoard[x0][y0] = Square{SquareOccupation::empty};
        newBoard[x0 + (x1-x0)/2][y0 + (y1-y0)/2] = Square{SquareOccupation::empty};
        newBoard[x1][y1] = Square{SquareOccupation::occupied};
        return {newBoard};
    }

    bool Board::isSolved() const noexcept
    {
        auto solutionFound{false};
        if (this->at(3,3).isOccupied() && (1 == this->getNumberOfPegsOnBoard()))
        {
            solutionFound = true;
        }
        return solutionFound;
    }
}
