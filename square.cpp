#include "square.h"

namespace solitaire
{
    Square::Square(SquareOccupation const initialOccupation) noexcept : occupation_{initialOccupation} {}
    bool Square::isOccupied() const noexcept {return SquareOccupation::occupied == occupation_;}

    std::string Square::occupationAsString() const noexcept
    {
        if (occupation_ == SquareOccupation::occupied)
        {
            return "X";
        }
        else if (occupation_ == SquareOccupation::empty)
        {
            return "O";
        }
        else
        {
            return " ";
        }
    }

    bool Square::isValidSquare() const noexcept
    {
        return (occupation_ != SquareOccupation::invalid);
    }
}
