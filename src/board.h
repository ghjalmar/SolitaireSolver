#ifndef SOLITAIREHACK_BOARD_H
#define SOLITAIREHACK_BOARD_H

#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <utility>

#include "square.h"

namespace solitaire
{
    constexpr std::uint8_t BoardWidth = 7;
    constexpr std::uint8_t BoardHeight = BoardWidth;

    using BoardRow = std::array<Square, BoardWidth>;
    using BoardType = std::array<BoardRow, BoardHeight>;
    using Row = std::int8_t;
    using Col = std::int8_t;
    using BoardIndex = std::pair<Row, Col>;
    using Move = std::pair<BoardIndex, BoardIndex>;
    using AvailableMoves = std::set<Move>;

    class Board
    {
    public:
        Board() noexcept;
        Board(BoardType board) noexcept;

        Square const& at(Row row, Col col) const noexcept;

        AvailableMoves getAvailableMoves() const noexcept;
        std::string getBoardState() const noexcept;
        std::uint8_t getNumberOfPegsOnBoard() const noexcept;

        Board applyMove(Move move) const noexcept;
        bool isSolved() const noexcept;
    private:
        Square invalidSquare_{SquareOccupation::invalid};
        BoardType board_;
    };
}

#endif //SOLITAIREHACK_BOARD_H
