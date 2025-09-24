
#include <iostream>
#include "Cards.h"

using namespace std;


// Definition of stream insertion operator overloading 
ostream& operator<<(ostream& os, const Card& c) {
    c.print(os); // Call overridden print function 
    return os;
}

// Do I need an istream operator overloading?
// istream& operator>>(istream& os, const Card& c){}

// Overriding of the play and print methods for all child Card classes 
void BombCard::play(){
    cout << "Played BombCard!" << endl;
}

void BombCard:: print(ostream& os) const {
    os << "This is a BombCard.";
}


void ReinforcementCard::play(){
    cout << "Played ReinforcementCard!" << endl;
}

void ReinforcementCard:: print(ostream& os) const {
    os << "This is a ReinforcementCard.";
}

void BlockadeCard::play(){
    cout << "Played BlockadeCard!" << endl;
}

void BlockadeCard:: print(ostream& os) const {
    os << "This is a BlockadeCard.";
}

void AirliftCard::play(){
    cout << "Played AirliftCard!" << endl;
}

void AirliftCard:: print(ostream& os) const {
    os << "This is a AirliftCard.";
}

void DiplomacyCard::play(){
    cout << "Played DiplomacyCard!" << endl;
}

void DiplomacyCard:: print(ostream& os) const {
    os << "This is a DiplomacyCard.";
}