
#include <iostream>
#include <vector>
#include <algorithm>
#include "Cards.h"

using namespace std;


// After assignment 1, include Player and Order h files 
// Temp Player and Order classes: 

ostream& operator<<(ostream& os, const Player& player){
    os << "Player's Order list contains: ";
    for (int i = 0; i < player.orders.size(); i++){
        os << *player.orders[i] << ", ";
    }
	return os; 
}


ostream& operator<<(ostream& os, const Order& order){
    os << "Order";
	return os; 
}


Player::Player(SpHand dealtHand)
//  : hand (dealtHand)
{
    hand = dealtHand;
}

void Player::addOrder(SpOrder order){
    cout << "Added order to player's list of orders \n";
    orders.push_back(order);
}

Order::Order(){
    cout << "Order created. \n";
}

// Implement operator overloading for Cards 
ostream& operator<<(ostream& os, const Card& c) {
    c.print(os); // Call overridden print function 
    return os;
}

// Card destructor
Card::~Card(){}
/**
 * Operator overloading 
 */
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
    out << "Deck contains: ";
    for (int i = 0; i < deck.spCards.size(); i++){
        out << *deck.spCards[i] << ", ";
    }
	return out; 
}

/**
 * Adds a card pointer to the deck if it does not already exist in it
 */
void Deck::add(SpCard card){
    auto it = find(spCards.begin(), spCards.end(), card); // find card pointer in hand
    if (it != spCards.end()){ 
        cout << "This " << *card << " object already exists in the deck!" << endl; //if exists, don't add another ref  
        return;
    }
    cout << "Added " << *card << " to the Deck." << endl;
    spCards.push_back(card);
}

/**
 * If it is not empty, draws a card from the top of the deck and adds it to the hand 
 */
SpCard Deck::draw(){
    if (spCards.empty()){
        cout << "The deck is empty" << endl;
        return SpCard(); // Return new null instance
    }
    
    SpCard card = spCards.back(); // make a copy of the card pointer at top of the deck 
    spCards.pop_back(); // remove card pointer at top of deck 
    cout << "Drew a " << *card << ". \n";
    return card;
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
 out << "Hand contains: ";
    for (int i = 0; i < hand.spCards.size(); i++){
        out << *hand.spCards[i] << ", ";
    }	
    return out; 
}

/**
 * Adds a card pointer to the hand if it does not already exist in it
 */
void Hand::add(SpCard card){
  auto it = find(spCards.begin(), spCards.end(), card); // find card pointer in hand
    if (it != spCards.end()){ 
        cout << "This " << *card << " object already exists in the deck!" << endl; //if exists, don't add another ref  
        return;
    }
    cout << "Added " << *card << " to the Hand." << endl;
    spCards.push_back(card);}

/**
 * Remove a card pointer from the hand
 */
SpCard Hand::remove(int index){
    if (index >= spCards.size() && index < 0){
        cout << "Invalid entry.";
        return SpCard();
    }

    SpCard card = spCards[index];
    spCards.erase(spCards.begin() + index);
    cout << "Removed card from the hand \n";
    return card;

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


void BombCard::play(Deck& deck, Hand& hand, Player& player){
    SpCard card = hand.remove(0);
    
    if (card){ // avoid null ref exceptions 
        cout << "Playing the top card of the hand, which is a " << *card << endl;
        SpOrder newOrder = SpOrder(new Order());  
        player.addOrder(newOrder); 
        deck.add(card);
    }
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

void ReinforcementCard::play(Deck& deck, Hand& hand, Player& player){
      SpCard card = hand.remove(0);
    
    if (card){ // avoid null ref exceptions 
        cout << "Playing the top card of the hand, which is a " << *card << endl;
        SpOrder newOrder = SpOrder(new Order());  
        player.addOrder(newOrder); 
        deck.add(card);
    }
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


void BlockadeCard::play(Deck& deck, Hand& hand, Player& player){
    SpCard card = hand.remove(0);
    
    if (card){ // avoid null ref exceptions 
        cout << "Playing the top card of the hand, which is a " << *card << endl;
        SpOrder newOrder = SpOrder(new Order());  
        player.addOrder(newOrder); 
        deck.add(card);
    }}

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


void AirliftCard::play(Deck& deck, Hand& hand, Player& player){
    SpCard card = hand.remove(0);
    
    if (card){ // avoid null ref exceptions 
        cout << "Playing the top card of the hand, which is a " << *card << endl;
        SpOrder newOrder = SpOrder(new Order());  
        player.addOrder(newOrder); 
        deck.add(card);
    }}

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

void DiplomacyCard::play(Deck& deck, Hand& hand, Player& player){
    SpCard card = hand.remove(0);
    
    if (card){ // avoid null ref exceptions 
        cout << "Playing the top card of the hand, which is a " << *card << endl;
        SpOrder newOrder = SpOrder(new Order());  
        player.addOrder(newOrder); 
        deck.add(card);
    }}

void DiplomacyCard:: print(ostream& os) const {
    os << "DiplomacyCard";
}