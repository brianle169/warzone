
#include <iostream>
#include <vector>
#include <algorithm>
#include "Cards.h"

using namespace std;

// Implement operator overloading for Cards 
ostream& operator<<(ostream& os, const Card& c) {
    c.print(os); // Call overridden print function 
    return os;
}

istream& operator>>(istream& is, const Card& c){
    // What do i even input?
    return is;
}

Card& Card::operator=(const Card&) {
    return *this;
}


// ------------Deck -------------
// Constructors 
Deck::Deck(){
    cout << "Deck created." << endl;
}

Deck::Deck(const Deck& other){
    cout << "Deck copied." << endl;
}

Deck::~Deck(){
    cout << "Deck destroyed." << endl;
}

// Operator overloading for Deck
/**
 * Assignment operator overload for Deck
 */
Deck& Deck::operator=(const Deck& other){
    if (this == &other) return *this;  // prevents bugs with self-assignment
    // handle any member deletion/reassignment here in the future 
    return *this;
}

/**
 * Output stream operator overload for Deck 
 */
ostream& operator<< (ostream& out, const Deck& deck) {
    out << "Deck size: " << deck.pcards.size() << ", contains: ";
    for (int i = 0; i < deck.pcards.size(); i++){
        out << *deck.pcards[i] << ", ";
    }
	return out; 
}

/**
 * Input stream operator overload for Deck 
 */
istream& operator>> (istream& in, const Deck& deck) {
	cout << "Enter ... what?";
	// in >> *();
	return in; 
}

/**
 * Adds a card pointer to the deck if it does not already exist in it
 */
void Deck::add(Card* card){
    auto it = find(pcards.begin(), pcards.end(), card); // find card pointer in hand
    if (it != pcards.end()){ 
        cout << "This " << *card << " object already exists in the deck!" << endl; //if exists, don't add another ref  
        return;
    }
    cout << "Added " << *card << " to the Deck." << endl;
    pcards.push_back(card);
}

/**
 * If it is not empty, draws a card from the top of the deck and adds it to the hand 
 */
void Deck::draw(Hand& hand){
    if (pcards.empty()){
        cout << "The deck is empty" << endl;
        return;
    }

    Card* card = pcards.front(); // make a copy of pointer to card (top card)
    pcards.erase(pcards.begin()); // erase the pointer in the deck (top card)
    cout << "Drew a " << *card << ". ";
    hand.add(card); // add the copy pointer to the hand
}


// ------------Hand -------------
// Constructors 
Hand::Hand(){
    cout << "Hand created." << endl;
}

Hand::Hand(const Hand& other){
    cout << "Hand copied." << endl;
}

Hand::~Hand(){
    // initialize future members?
    cout << "Hand destroyed." << endl;
}

// Operator overloading for Hand
/**
 * Assignment operator overload for Deck
 */
Hand& Hand::operator=(const Hand& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    // handle any member deletion/reassignment here in the future 
    return *this;
}

/**
 * Output stream operator overload for Hand 
 */
ostream& operator<< (ostream& out, const Hand& hand) {
 out << "Hand size: " << hand.pcards.size() << ", contains: ";
    for (int i = 0; i < hand.pcards.size(); i++){
        out << *hand.pcards[i] << ", ";
    }	
    return out; 
}

/**
 * Input stream operator overload for Hand 
 */
istream& operator>> (istream& in, const Hand& hand) {
	cout << "Enter ... what?";
	// in >> *();
	return in; 
}

/**
 * Adds a card pointer to the hand if it does not already exist in it
 */
void Hand::add(Card* card){
  auto it = find(pcards.begin(), pcards.end(), card); // find card pointer in hand
    if (it != pcards.end()){ 
        cout << "This " << *card << " object already exists in the deck!" << endl; //if exists, don't add another ref  
        return;
    }
    cout << "Added " << *card << " to the Hand." << endl;
    pcards.push_back(card);}

/**
 * Remove a card pointer from the hand
 */
void Hand::remove(Card* card){
    auto it = find(pcards.begin(), pcards.end(), card); // find card pointer in hand
    if (it != pcards.end()){ 
        pcards.erase(it); //if exists, remove pointer 
    }
}

/**
 * Getter for cards in hand
 */
const vector<Card*>& Hand:: GetCards() const {
    return pcards;
}



// Definitions for all child Card classes 

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

void BombCard::play(Deck& deck, Hand& hand){
    auto it = find(hand.GetCards().begin(), hand.GetCards().end(), this); // find card pointer in hand
    if (it == hand.GetCards().end()){ 
        cout << "This " << *this << " is not in the Hand, so you can't play it!" << endl; 
        return;
    }
    cout << "Played BombCard! ";
    hand.remove(this);
    deck.add(this);
}

void BombCard:: print(ostream& os) const {
    os << "BombCard";
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

void ReinforcementCard::play(Deck& deck, Hand& hand){
    auto it = find(hand.GetCards().begin(), hand.GetCards().end(), this); // find card pointer in hand
    if (it == hand.GetCards().end()){ 
        cout << "This card is not in the Hand, so you can't play it!" << endl; 
        return;
    }
    cout << "Played ReinforcementCard! ";
    hand.remove(this);
    deck.add(this);
}

void ReinforcementCard:: print(ostream& os) const {
    os << "ReinforcementCard";
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

void BlockadeCard::play(Deck& deck, Hand& hand){
    auto it = find(hand.GetCards().begin(), hand.GetCards().end(), this); // find card pointer in hand
    if (it == hand.GetCards().end()){ 
        cout << "This card is not in the Hand, so you can't play it!" << endl; 
        return;
    }
    cout << "Played BlockadeCard! ";
    hand.remove(this);
    deck.add(this);
}

void BlockadeCard:: print(ostream& os) const {
    os << "BlockadeCard";
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

void AirliftCard::play(Deck& deck, Hand& hand){
    auto it = find(hand.GetCards().begin(), hand.GetCards().end(), this); // find card pointer in hand
    if (it == hand.GetCards().end()){ 
        cout << "This card is not in the Hand, so you can't play it!" << endl; 
        return;
    }
    cout << "Played AirliftCard! ";
    hand.remove(this);
    deck.add(this);
}

void AirliftCard:: print(ostream& os) const {
    os << "AirliftCard";
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

void DiplomacyCard::play(Deck& deck, Hand& hand){
    auto it = find(hand.GetCards().begin(), hand.GetCards().end(), this); // find card pointer in hand
    if (it == hand.GetCards().end()){ 
        cout << "This card is not in the Hand, so you can't play it!" << endl; 
        return;
    }
    cout << "Played DiplomacyCard! ";
    hand.remove(this);
    deck.add(this);
}

void DiplomacyCard:: print(ostream& os) const {
    os << "DiplomacyCard";
}

