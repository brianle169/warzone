
#include <iostream>
#include "Cards.h"

using namespace std;

Card::Card() {
    type = CardType::Bomb;
}

void Card::play() {
    std::cout << "Playing card!" << std::endl;
}

// int main(){

//     Card card;
//     card.play();

// }

    

