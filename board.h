#ifndef SOLITAIREHACK_BOARD_H
#define SOLITAIREHACK_BOARD_H

#include <cstdint>
#include <string>
#include <vector>

namespace solitaire
{
    constexpr std::uint8_t boardWidth = 7;
    constexpr std::uint8_t boardHeight = boardWidth;

    class Square
    {
    public:
        Square() noexcept;
        Square(bool const initialOccupation) noexcept;
        bool isOccupied() const noexcept;
        std::string isOccupiedAsString() const noexcept;
        void toggle() noexcept;

    private:
        bool occupied_{true};
    };

    using BoardRow = std::vector<Square>;
    using BoardType = std::vector<BoardRow>;

    class Board
    {
    public:
        Board() noexcept;

        std::string getBoardState() const noexcept;
    private:
        BoardType board_;
    };
}

#endif //SOLITAIREHACK_BOARD_H
