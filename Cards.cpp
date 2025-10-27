
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include "Cards.h"
#include "Player.h"


using namespace std;

// Implement operator overloading for Cards 
ostream& operator<<(ostream& os, const Card& c) {
    c.print(os); // Call overridden print function 
    return os;
}

// Card destructor
Card::~Card(){}

// Operator overloading 
Card& Card::operator=(const Card&) {
    return *this;
}

// Removes a card from the hand, creates a new list of orders and adds it to the player's list of orders, and adds the card back to the deck 
void Card::play(Deck& deck, Hand& hand, Player& player){
    int index;
    SpCard card;
    while (true){
        cout << "Choose the index of the card you wish to play: " ;
        cin >> index;
        // Check if input is an int
        if (cin.fail()){
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid entry. " ;
            continue;  
        } 
        // Remove card from hand and place in the deck
        card = hand.remove(index);
        if (card) {
            deck.add(card);
            break;
        }
    }

}

// ------------Deck -------------

// Default constructor 
Deck::Deck(){
    cout << "Deck created." << endl;
}

// Copy constructor 
Deck::Deck(const Deck& other) : spCards(other.spCards) {
    cout << "Deck copied." << endl;
}

// Destructor 
Deck::~Deck(){
    cout << "Deck destroyed." << endl;
}

// Operator overloading for Deck
Deck& Deck::operator=(const Deck& other){
    if (this == &other) return *this;  // prevents bugs with self-assignment
    spCards = other.spCards;
    // handle any member deletion/reassignment here in the future 
    return *this;
}

// Output stream operator overload for Deck 
ostream& operator<< (ostream& out, const Deck& deck) {
    out << "Deck contains: ";
    for (int i = 0; i < deck.spCards.size(); i++){
        out << *deck.spCards[i] << ", ";
    }
	return out; 
}

// Adds a card pointer to the deck if it does not already exist in it
void Deck::add(SpCard card){
    auto it = find(spCards.begin(), spCards.end(), card); // find card pointer in hand
    if (it != spCards.end()){ 
        cout << "This " << *card << " object already exists in the deck!" << endl; //if exists, don't add another ref  
        return;
    }
    cout << "Added " << *card << " to the Deck." << endl;
    spCards.push_back(card);
}


// If it is not empty, draws a card from the top of the deck and returns it
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
// Default constructor
Hand::Hand(){
    cout << "Hand created." << endl;
}
// Copy constructor 
Hand::Hand(const Hand& other) : spCards(other.spCards) {
    cout << "Hand copied." << endl;
}
//Destructor 
Hand::~Hand(){
    // initialize future members?
    cout << "Hand destroyed." << endl;
}

// Operator overloading for Hand
Hand& Hand::operator=(const Hand& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    spCards = other.spCards;
    // handle any member deletion/reassignment here in the future 
    return *this;
}

// Output stream operator overload for Hand 
ostream& operator<< (ostream& out, const Hand& hand) {
 out << "Hand contains: ";
    for (int i = 0; i < hand.spCards.size(); i++){
        out << "[" << i << "] " << *hand.spCards[i] << ", ";
    }	
    return out; 
}

// Adds a card pointer to the hand if it does not already exist in it
void Hand::add(SpCard card){
  auto it = find(spCards.begin(), spCards.end(), card); // find card pointer in hand
    if (it != spCards.end()){ 
        cout << "This " << *card << " object already exists in the deck!" << endl; //if exists, don't add another ref  
        return;
    }
    cout << "Added " << *card << " to the Hand." << endl;
    spCards.push_back(card);}

// Remove a card pointer from the hand
SpCard Hand::remove(int index){
    if (index >= spCards.size() || index < 0){
        cout << "Invalid entry. ";
        return SpCard();
    }

    SpCard card = spCards[index];
    spCards.erase(spCards.begin() + index);
    cout << "Removed " << *card << " from the hand \n";
    return card;

}

// Searches hand for card name
bool Hand::includes(string name) {
    for(SpCard card: spCards) {
        if(card->getName() == name) {
            return true;
        }
    }
    return false;
}

// Definitions for all child Card classes 

// ------------BombCard-------------

// Default constructor 
BombCard::BombCard(){
    cout << "BombCard created." << endl;
}

// Copy constructor 
BombCard::BombCard(const BombCard& other){
    cout << "BombCard copied." << endl;
}
// Destructor 
BombCard::~BombCard(){
    cout << "BombCard destroyed." << endl;
}

// Assignment operator 
BombCard& BombCard::operator=(const BombCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream 
void BombCard:: print(ostream& os) const {
    os << "BombCard";
}

// Return card name
string BombCard::getName() const{
    return "Bomb";
}


//------------ReinforcementCard----------------

// Default constructor 
ReinforcementCard::ReinforcementCard(){
    cout << "ReinforcementCard created." << endl;
}

// Copy constructor 
ReinforcementCard::ReinforcementCard(const ReinforcementCard& other){
    cout << "ReinforcementCard copied." << endl;
}

// Destructor 
ReinforcementCard::~ReinforcementCard(){
    cout << "ReinforcementCard destroyed." << endl;
}

// Assignment operator 
ReinforcementCard& ReinforcementCard::operator=(const ReinforcementCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream 
void ReinforcementCard:: print(ostream& os) const {
    os << "ReinforcementCard";
}

// Return card name
string ReinforcementCard::getName() const{
    return "Reinforcement";
}


//------------BlockadeCard----------------

// Default constructor 
BlockadeCard::BlockadeCard(){
    cout << "BlockadeCard created." << endl;
}

// Copy constructor 
BlockadeCard::BlockadeCard(const BlockadeCard& other){
    cout << "BlockadeCard copied." << endl;
}

// Destructor 
BlockadeCard::~BlockadeCard(){
    cout << "BlockadeCard destroyed." << endl;
}

// Assignment operator 
BlockadeCard& BlockadeCard::operator=(const BlockadeCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream 
void BlockadeCard:: print(ostream& os) const {
    os << "BlockadeCard";
}

// Return card name
string BlockadeCard::getName() const{
    return "Blockade";
}

//------------AirliftCard----------------

// Default constructor 
AirliftCard::AirliftCard(){
    cout << "AirliftCard created." << endl;

}

// Copy constructor 
AirliftCard::AirliftCard(const AirliftCard& other){
    cout << "AirliftCard copied." << endl;
}

// Destructor 
AirliftCard::~AirliftCard(){
    cout << "AirliftCard destroyed." << endl;
}

// Assignment operator 
AirliftCard& AirliftCard::operator=(const AirliftCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream 
void AirliftCard:: print(ostream& os) const {
    os << "AirliftCard";
}

// Return card name
string AirliftCard::getName() const{
    return "Airlift";
}

//------------DiplomacyCard----------------

// Default constructor 
DiplomacyCard::DiplomacyCard(){
    cout << "DiplomacyCard created." << endl;
}

// Copy constructor 
DiplomacyCard::DiplomacyCard(const DiplomacyCard& other){
    cout << "DiplomacyCard copied." << endl;
}

// Destructor 
DiplomacyCard::~DiplomacyCard(){
    cout << "DiplomacyCard destroyed." << endl;
}

// Assignment operator 
DiplomacyCard& DiplomacyCard::operator=(const DiplomacyCard& other){
    if (this == &other){ // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream 
void DiplomacyCard:: print(ostream& os) const {
    os << "DiplomacyCard";
}

// Return card name
string DiplomacyCard::getName() const{
    return "Negotiate";
}