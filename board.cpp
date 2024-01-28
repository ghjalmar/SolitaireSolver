#include <algorithm>

#include "board.h"

namespace solitaire
{
    Square::Square() noexcept : occupied_{true} {}
    Square::Square(const bool initialOccupation) noexcept : occupied_{initialOccupation} {}
    void Square::toggle() noexcept {occupied_ = !occupied_;}
    bool Square::isOccupied() const noexcept {return occupied_;}
    std::string Square::isOccupiedAsString() const noexcept
    {
        if (occupied_)
        {
            return "X";
        }
        else
        {
            return "O";
        }
    }

    Board::Board() noexcept
    {
        for (std::uint8_t row{0}; row < boardHeight; ++row)
        {
            if (row < 2 || row > 4)
            {
                board_.push_back({Square{}, Square{}, Square{}});
            }
            else if (3 == row)
            {
                board_.push_back({Square{}, Square{}, Square{}, Square{false}, Square{}, Square{}, Square{}});
            }
            else
            {
                board_.push_back({Square{}, Square{}, Square{}, Square{}, Square{}, Square{}, Square{}});
            }
        }
    }

    std::string Board::getBoardState() const noexcept
    {
        std::string boardState{};
        for (std::uint8_t row{0}; row < boardHeight; ++row)
        {
            std::string line{};
            std::for_each(board_.at(row).cbegin(), board_.at(row).cend(),
                          [&line](Square square){line.append(std::string{square.isOccupiedAsString()});});
            if (row < 2 || row > 4)
            {
                boardState.append("  ");
                boardState.append(line);
                boardState.append("  ");
            }
            else
            {
                boardState.append(line);
            }
            boardState.append("\n");
        }
        return boardState;
    }
}
