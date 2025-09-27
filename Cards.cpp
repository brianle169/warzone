
#include <iostream>
#include "Cards.h"

using namespace std;


// Definition of stream insertion operator overloading 
ostream& operator<<(ostream& os, const Card& c) {
    c.print(os); // Call overridden print function 
    return os;
}

Card& Card::operator=(const Card&) {
    return *this;
}

// Do I need an istream operator overloading?
// istream& operator>>(istream& os, const Card& c){}

// Overriding of the play and print methods for all child Card classes 

// ------------BombCard-------------

BombCard::BombCard(){
    // initialize future members?
    cout << "BombCard created." << endl;

}

BombCard::BombCard(const BombCard& other){
    // initialize future members?
    cout << "BombCard copied." << endl;

}

BombCard::~BombCard(){
    // initialize future members?
    cout << "BombCard destroyed." << endl;

}

BombCard& BombCard::operator=(const BombCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    // Make sure to handle member deletion/copying here in the future
    return *this;
}

Card* BombCard::clone() const {
    return new BombCard(*this);
}

void BombCard::play(){
    cout << "Played BombCard!" << endl;
}

void BombCard:: print(ostream& os) const {
    os << "This is a BombCard.";
}


//------------ReinforcementCard----------------

ReinforcementCard::ReinforcementCard(){
    // initialize future members?
    cout << "ReinforcementCard created." << endl;

}

ReinforcementCard::ReinforcementCard(const ReinforcementCard& other){
    // initialize future members?
    cout << "ReinforcementCard copied." << endl;

}

ReinforcementCard::~ReinforcementCard(){
    // initialize future members?
    cout << "ReinforcementCard destroyed." << endl;
}

ReinforcementCard& ReinforcementCard::operator=(const ReinforcementCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    // Make sure to handle member deletion/copying here in the future
    return *this;
}

Card* ReinforcementCard::clone() const {
    return new ReinforcementCard(*this);
}

void ReinforcementCard::play(){
    cout << "Played ReinforcementCard!" << endl;
}

void ReinforcementCard:: print(ostream& os) const {
    os << "This is a ReinforcementCard.";
}


//------------BlockadeCard----------------


BlockadeCard::BlockadeCard(){
    // initialize future members?
    cout << "BlockadeCard created." << endl;

}

BlockadeCard::BlockadeCard(const BlockadeCard& other){
    // initialize future members?
    cout << "BlockadeCard copied." << endl;

}

BlockadeCard::~BlockadeCard(){
    // initialize future members?
    cout << "BlockadeCard destroyed." << endl;

}

BlockadeCard& BlockadeCard::operator=(const BlockadeCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    // Make sure to handle member deletion/copying here in the future
    return *this;
}

Card* BlockadeCard::clone() const {
    return new BlockadeCard(*this);
}

void BlockadeCard::play(){
    cout << "Played BlockadeCard!" << endl;
}

void BlockadeCard:: print(ostream& os) const {
    os << "This is a BlockadeCard.";
}

//------------AirliftCard----------------


AirliftCard::AirliftCard(){
    // initialize future members?
    cout << "AirliftCard created." << endl;

}

AirliftCard::AirliftCard(const AirliftCard& other){
    // initialize future members?
    cout << "AirliftCard copied." << endl;

}

AirliftCard::~AirliftCard(){
    // initialize future members?
    cout << "AirliftCard destroyed." << endl;

}

AirliftCard& AirliftCard::operator=(const AirliftCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    // Make sure to handle member deletion/copying here in the future
    return *this;
}

Card* AirliftCard::clone() const {
    return new AirliftCard(*this);
}

void AirliftCard::play(){
    cout << "Played AirliftCard!" << endl;
}

void AirliftCard:: print(ostream& os) const {
    os << "This is a AirliftCard.";
}

//------------DiplomacyCard----------------

DiplomacyCard::DiplomacyCard(){
    // initialize future members?
    cout << "DiplomacyCard created." << endl;

}

DiplomacyCard::DiplomacyCard(const DiplomacyCard& other){
    // initialize future members?
    cout << "DiplomacyCard copied." << endl;

}

DiplomacyCard::~DiplomacyCard(){
    // initialize future members?
    cout << "DiplomacyCard destroyed." << endl;

}

DiplomacyCard& DiplomacyCard::operator=(const DiplomacyCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    // Make sure to handle member deletion/copying here in the future
    return *this;
}

Card* DiplomacyCard::clone() const {
    return new DiplomacyCard(*this);
}

void DiplomacyCard::play(){
    cout << "Played DiplomacyCard!" << endl;
}

void DiplomacyCard:: print(ostream& os) const {
    os << "This is a DiplomacyCard.";
}

