#include <iostream>

#include "board.h"

int main() {
    std::cout << "Halló heimur!" << std::endl;
    auto bord = solitaire::Board();
    std::cout << "Upprunaleg staða á borði: " << std::endl;
    std::cout << bord.getBoardState() << std::endl;
    return 0;
}
