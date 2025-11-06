
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>

#include "Cards.h"
#include "Player.h"
#include "GameEngine.h"

using namespace std;

// Implement operator overloading for Cards
ostream &operator<<(ostream &os, const Card &c)
{
    c.print(os); // Call overridden print function
    return os;
}

// Card destructor
Card::~Card() {}

// Operator overloading
Card &Card::operator=(const Card &)
{
    return *this;
}

// ------------Deck -------------

// Default constructor
Deck::Deck()
{
    cout << "Deck created." << endl;
}

// Copy constructor
Deck::Deck(const Deck &other) : spCards(other.spCards)
{
    cout << "Deck copied." << endl;
}

// Destructor
Deck::~Deck()
{
    cout << "Deck destroyed." << endl;
}

// Operator overloading for Deck
Deck &Deck::operator=(const Deck &other)
{
    if (this == &other)
        return *this; // prevents bugs with self-assignment
    spCards = other.spCards;
    // handle any member deletion/reassignment here in the future
    return *this;
}

// Output stream operator overload for Deck
ostream &operator<<(ostream &out, const Deck &deck)
{
    out << "Deck contains: ";
    for (int i = 0; i < deck.spCards.size(); i++)
    {
        out << *deck.spCards[i] << ", ";
    }
    return out;
}

// Adds a card pointer to the deck if it does not already exist in it
void Deck::add(SpCard card)
{
    auto it = find(spCards.begin(), spCards.end(), card); // find card pointer in hand
    if (it != spCards.end())
    {
        cout << "This " << *card << " object already exists in the deck!" << endl; // if exists, don't add another ref
        return;
    }
    cout << "Added " << *card << " to the Deck." << endl;
    spCards.push_back(card);
}

// If it is not empty, draws a card from the top of the deck and returns it
SpCard Deck::draw()
{
    if (spCards.empty())
    {
        cout << "The deck is empty" << endl;
        return SpCard(); // Return new null instance
    }

    SpCard card = spCards.back(); // make a copy of the card pointer at top of the deck
    spCards.pop_back();           // remove card pointer at top of deck
    cout << "Drew a " << *card << ". \n";
    return card;
}

// ------------Hand -------------
// Default constructor
Hand::Hand()
{
    cout << "Hand created." << endl;
}
// Copy constructor
Hand::Hand(const Hand &other) : spCards(other.spCards)
{
    cout << "Hand copied." << endl;
}
// Destructor
Hand::~Hand()
{
    // initialize future members?
    cout << "Hand destroyed." << endl;
}

// Operator overloading for Hand
Hand &Hand::operator=(const Hand &other)
{
    if (this == &other)
    { // prevents bugs with self-assignment
        return *this;
    }
    spCards = other.spCards;
    // handle any member deletion/reassignment here in the future
    return *this;
}

// Output stream operator overload for Hand
ostream &operator<<(ostream &out, const Hand &hand)
{
    out << "Hand contains: ";
    for (int i = 0; i < hand.spCards.size(); i++)
    {
        out << "[" << i << "] " << *hand.spCards[i] << ", ";
    }
    return out;
}

// Adds a card pointer to the hand if it does not already exist in it
void Hand::add(SpCard card)
{
    auto it = find(spCards.begin(), spCards.end(), card); // find card pointer in hand
    if (it != spCards.end())
    {
        cout << "This " << *card << " object already exists in the deck!" << endl; // if exists, don't add another ref
        return;
    }
    cout << "Added " << *card << " to the Hand." << endl;
    spCards.push_back(card);
}

// Remove a card pointer from the hand
SpCard Hand::remove(int index)
{
    if (index >= spCards.size() || index < 0)
    {
        cout << "Invalid entry. ";
        return SpCard();
    }

    SpCard card = spCards[index];
    spCards.erase(spCards.begin() + index);
    cout << "Removed " << *card << " from the hand \n";
    return card;
}

// Searches hand for card name
bool Hand::includes(string name)
{
    for (SpCard card : spCards)
    {
        if (card->getName() == name)
        {
            return true;
        }
    }
    return false;
}

int Hand::getNumCards()
{
    return spCards.size();
}

SpCard Hand::getCardAt(int index)
{
    if (index >= 0 && index < spCards.size())
    {
        return spCards[index];
    }
    return SpCard(); // Return null shared_ptr if index is out of bounds
}

// Definitions for all child Card classes

// ------------BombCard-------------

// Default constructor
BombCard::BombCard()
{
    cout << "BombCard created." << endl;
}

// Copy constructor
BombCard::BombCard(const BombCard &other)
{
    cout << "BombCard copied." << endl;
}
// Destructor
BombCard::~BombCard()
{
    cout << "BombCard destroyed." << endl;
}

// Assignment operator
BombCard &BombCard::operator=(const BombCard &other)
{
    if (this == &other)
    { // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream
void BombCard::print(ostream &os) const
{
    os << "BombCard";
}

// Return card name
string BombCard::getName() const
{
    return "Bomb";
}

void BombCard::play(Deck &deck, Player &player)
{
    // The player can choose any territory on the map. The Bomb order class will validate if the territory can be bombed.
    cout << "Bomb Card: Choose a territory to bomb." << endl;
    // Implementation for bombing a territory goes here
    cout << "Here are all the territories of the map:" << endl;
    cout << *GameEngine::getGameMap() << endl;
    string territoryName;
    Territory *targetTerritory = nullptr;
    while (true)
    {
        cout << "Enter the name of the territory to bomb >> ";
        cin >> territoryName;
        targetTerritory = GameEngine::getGameMap()->getTerritory(territoryName);
        if (targetTerritory)
            break;
        else
            cout << "Invalid territory name. Please try again." << endl;
    }
    player.getOrdersList()->addOrder(new Bomb(&player, targetTerritory, nullptr));
}

//------------ReinforcementCard----------------

// Default constructor
ReinforcementCard::ReinforcementCard()
{
    cout << "ReinforcementCard created." << endl;
}

// Copy constructor
ReinforcementCard::ReinforcementCard(const ReinforcementCard &other)
{
    cout << "ReinforcementCard copied." << endl;
}

// Destructor
ReinforcementCard::~ReinforcementCard()
{
    cout << "ReinforcementCard destroyed." << endl;
}

// Assignment operator
ReinforcementCard &ReinforcementCard::operator=(const ReinforcementCard &other)
{
    if (this == &other)
    { // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream
void ReinforcementCard::print(ostream &os) const
{
    os << "ReinforcementCard";
}

// Return card name
string ReinforcementCard::getName() const
{
    return "Reinforcement";
}

//------------BlockadeCard----------------

// Default constructor
BlockadeCard::BlockadeCard()
{
    cout << "BlockadeCard created." << endl;
}

// Copy constructor
BlockadeCard::BlockadeCard(const BlockadeCard &other)
{
    cout << "BlockadeCard copied." << endl;
}

// Destructor
BlockadeCard::~BlockadeCard()
{
    cout << "BlockadeCard destroyed." << endl;
}

// Assignment operator
BlockadeCard &BlockadeCard::operator=(const BlockadeCard &other)
{
    if (this == &other)
    { // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream
void BlockadeCard::print(ostream &os) const
{
    os << "BlockadeCard";
}

// Return card name
string BlockadeCard::getName() const
{
    return "Blockade";
}

void BlockadeCard::play(Deck &deck, Player &player)
{
    // The player can choose any territory they own. The Blockade order class will validate if the territory can be blockaded.
    cout << "Blockade Card: Choose a territory to blockade." << endl;
    cout << "Here are your territories:" << endl;
    player.displayTerritories(*player.getTerritories());
    string territoryName;
    Territory *targetTerritory = nullptr;
    while (true)
    {
        cout << "Enter the name of the territory to blockade >> ";
        cin >> territoryName;
        targetTerritory = GameEngine::getGameMap()->getTerritory(territoryName);
        // Note that the validation step that checks if the player owns the territory is done in the Blockade order's validate() method
        // Here, we just need to ensure the territory exists
        if (targetTerritory)
            break;
        else
            cout << "Invalid territory name. Please try again." << endl;
    }
    player.getOrdersList()->addOrder(new Blockade(&player, targetTerritory));
}

//------------AirliftCard----------------

// Default constructor
AirliftCard::AirliftCard()
{
    cout << "AirliftCard created." << endl;
}

// Copy constructor
AirliftCard::AirliftCard(const AirliftCard &other)
{
    cout << "AirliftCard copied." << endl;
}

// Destructor
AirliftCard::~AirliftCard()
{
    cout << "AirliftCard destroyed." << endl;
}

// Assignment operator
AirliftCard &AirliftCard::operator=(const AirliftCard &other)
{
    if (this == &other)
    { // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream
void AirliftCard::print(ostream &os) const
{
    os << "AirliftCard";
}

// Return card name
string AirliftCard::getName() const
{
    return "Airlift";
}

void AirliftCard::play(Deck &deck, Player &player)
{
    // The player can choose any of their territories to airlift from and to any other territory they own. The Airlift order class will validate if the airlift can occur.
    cout << "Airlift Card: Choose territories to airlift from and to." << endl;
    cout << "Here are your territories:" << endl;
    player.displayTerritories(*player.getTerritories());
    string fromTerritoryName, toTerritoryName;
    int numArmies = 0;
    Territory *fromTerritory = nullptr;
    Territory *toTerritory = nullptr;
    while (true)
    {
        cout << "Enter the name of the territory to airlift from >> ";
        cin >> fromTerritoryName;
        fromTerritory = GameEngine::getGameMap()->getTerritory(fromTerritoryName);
        // Note that the validation step that checks if the player owns the territory is done in the Airlift order's validate() method
        // Here, we just need to ensure the territory exists
        if (fromTerritory)
            break;
        else
            cout << "Invalid territory name. Please try again." << endl;
    }
    while (true)
    {
        cout << "Enter the number of armies to airlift >> ";
        cin >> numArmies;
        if (cin.fail() || numArmies <= 0 || numArmies > fromTerritory->getArmies())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number of armies. Please try again." << endl;
            continue;
        }
        break;
    }
    while (true)
    {
        cout << "Enter the name of the territory to airlift to >> ";
        cin >> toTerritoryName;
        toTerritory = GameEngine::getGameMap()->getTerritory(toTerritoryName);
        // Note that the validation step that checks if the player owns the territory is done in the Airlift order's validate() method
        // Here, we just need to ensure the territory exists
        if (toTerritory)
            break;
        else
            cout << "Invalid territory name. Please try again." << endl;
    }
    player.getOrdersList()->addOrder(new Airlift(&player, numArmies, fromTerritory, toTerritory)); // Number of armies will be set in Airlift order execution
}

//------------DiplomacyCard----------------

// Default constructor
DiplomacyCard::DiplomacyCard()
{
    cout << "DiplomacyCard created." << endl;
}

// Copy constructor
DiplomacyCard::DiplomacyCard(const DiplomacyCard &other)
{
    cout << "DiplomacyCard copied." << endl;
}

// Destructor
DiplomacyCard::~DiplomacyCard()
{
    cout << "DiplomacyCard destroyed." << endl;
}

// Assignment operator
DiplomacyCard &DiplomacyCard::operator=(const DiplomacyCard &other)
{
    if (this == &other)
    { // prevents bugs with self-assignment
        return *this;
    }
    return *this;
}

// Override print method to output stream
void DiplomacyCard::print(ostream &os) const
{
    os << "DiplomacyCard";
}

// Return card name
string DiplomacyCard::getName() const
{
    return "Negotiate";
}

void DiplomacyCard::play(Deck &deck, Player &player)
{
    // The player can choose any other player to negotiate with. The Negotiate order class will validate if the negotiation can occur.
    cout << "Diplomacy Card: Choose a player to negotiate with." << endl;
    cout << "Here are all the players in the game:" << endl;
    for (Player *p : GameEngine::getPlayers())
    {
        if (p != &player)
            cout << p->getName() << endl;
    }
    string playerName;
    Player *targetPlayer = nullptr;
    while (true)
    {
        cout << "Enter the name of the player to negotiate with >> ";
        cin >> playerName;
        // Find player by name
        for (Player *p : GameEngine::getPlayers())
        {
            if (p->getName() == playerName && p != &player)
            {
                targetPlayer = p;
                break;
            }
        }
        if (targetPlayer)
            break;
        else
            cout << "Invalid player name. Please try again." << endl;
    }
    player.getOrdersList()->addOrder(new Negotiate(&player, targetPlayer));
}