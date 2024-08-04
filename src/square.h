#ifndef SOLITAIREHACK_SQUARE_H
#define SOLITAIREHACK_SQUARE_H

#include <string>

namespace solitaire
{
    enum class SquareOccupation : int
    {
        invalid = 0,
        empty = 1,
        occupied = 2
    };

    class Square final
    {
    public:
        Square() noexcept = default;
        Square(SquareOccupation initialOccupation) noexcept;

        bool isOccupied() const noexcept;
        std::string occupationAsString() const noexcept;

        bool isValidSquare() const noexcept;

    private:
        SquareOccupation occupation_{SquareOccupation::invalid};
    };
}

#endif //SOLITAIREHACK_SQUARE_H
