#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include <iostream>
#include <algorithm>
using namespace std;

//ORDER CLASS DEFINITION---------------------------------------------------

// Constructor initializing order with name and player
Order::Order(std::string orderN, Player *p) {
    orderName=orderN;
    player=p;
}

// Copy constructor for Order
Order::Order(const Order &other) {
    orderName=other.orderName;
    this->player=other.player;
}

// Assignment operator for Order
Order& Order::operator=(const Order &other) {
    if (this != &other) {
        orderName=other.orderName;
        this->player= other.player;
    }
    return *this;
}

// Output stream for displaying order execution
ostream& operator<<(ostream& os, const Order& order) {
    os << order.getPlayer() << ": " << order.getName();
    if (order.executed) {
        os << " EXECUTED -> " << order.executionEffect;
    }
    return os;
}

// Setter for execution effect description
void Order::setExecutionEffect(const std::string& effect) {
    executionEffect = effect;
}

string Order::getPlayer() const {
    return player ? player->getName() : std::string("<null>");
}

//DEPLOY CLASS DEFINITON---------------------------------------------------

// Constructor for Deploy order
Deploy::Deploy(Player* p,Territory* targetT, int numA){
    player=p;
    targetTerritory=targetT;
    numArmies=numA;
}

// Copy constructor for Deploy
Deploy::Deploy(const Deploy& other) : Order(other) {
    numArmies = other.numArmies;
    targetTerritory = other.targetTerritory;
}

// Assignment operator for Deploy
Deploy& Deploy::operator=(const Deploy& other) {
    if (this != &other) {
        Order::operator=(other);
        numArmies = other.numArmies;
        targetTerritory = other.targetTerritory;
    }
    return *this;
}

// Output stream for Deploy info
ostream& operator<<(ostream& os, const Deploy& d) {
    os << "Deploy " << d.numArmies << " armies to territory " << d.targetTerritory->getName();
    return os;
}

// Validates if Deploy order can execute
bool Deploy::validate() {
    return player == targetTerritory->getPlayer() && numArmies > 0;
}

// Executes the Deploy order
void Deploy::execute() {
    if (validate()) {
        targetTerritory->setArmies(targetTerritory->getArmies()+numArmies);
        executed = true;
        setExecutionEffect("Successfully deployed " + to_string(numArmies) + " armies to " + targetTerritory->getName() + "; " + targetTerritory->getName() + " has now " + to_string(targetTerritory->getArmies()) + " troops");
    }
}

Order* Deploy::clone() const {
    return new Deploy(*this);
}

std::string Deploy::getName() const{
    return "Deploy";
}

//ADVANCE CLASS DEFINITION---------------------------------------------------

// Constructor for Advance order
Advance::Advance(Player* p, int moveNumArmy, Territory *baseTerritory, Territory *wantedTerritory) {
    player=p;
    numArmy = moveNumArmy;
    sourceTerritory = baseTerritory;
    targetTerritory = wantedTerritory;
}

// Copy constructor
Advance::Advance(const Advance &other) : Order(other) {
    numArmy = other.numArmy;
    sourceTerritory = other.sourceTerritory;
    targetTerritory = other.targetTerritory;
}

// Assignment operator
Advance& Advance::operator=(const Advance &other) {
    if (this != &other) {
        Order::operator=(other);
        numArmy = other.numArmy;
        sourceTerritory = other.sourceTerritory;
        targetTerritory = other.targetTerritory;
    }
    return *this;
}

// Output stream for Advance
ostream& operator<<(ostream& os, const Advance& a) {
    os << "Advance " << a.numArmy << " from " << a.sourceTerritory << " to " << a.targetTerritory;
    return os;
}

// Checks if the move is valid
bool Advance::validate() {
    return sourceTerritory->getArmies() > numArmy && numArmy > 0 && targetTerritory->isEdge(sourceTerritory);
}

// Executes troop movement and combat logic
void Advance::execute() {
    if (validate()) {
        if (player == targetTerritory->getPlayer()) { //the player wants to advance troops on hiw own territory
            sourceTerritory->setArmies(sourceTerritory->getArmies()-numArmy);
            targetTerritory->setArmies(targetTerritory->getArmies()+numArmy);
            executed = true;
            setExecutionEffect("Successfully advanced " + to_string(numArmy) + " troops from " + sourceTerritory->getName() + " to " + targetTerritory->getName() + "; " + targetTerritory->getName() + " has now " + to_string(targetTerritory->getArmies()) + " troops");
        }

        if (player != targetTerritory->getPlayer()) { //the player successfully conquered the territory
            if (numArmy*0.6 > targetTerritory->getArmies()*0.7) {
                int result = numArmy*0.6 - targetTerritory->getArmies()*0.7;
                sourceTerritory->setArmies(sourceTerritory->getArmies()-numArmy);
                targetTerritory->setArmies(result);
                targetTerritory->setPlayer(player);
                //add a card to the deck since conquered
                executed = true;
                setExecutionEffect("Successfully conquered and advanced " + to_string(numArmy) + " troops from " + sourceTerritory->getName() + " to " + targetTerritory->getName() + "; "+ sourceTerritory->getName() + " has now " + to_string(sourceTerritory->getArmies()) + " troops and "  + targetTerritory->getName() + " has now " + to_string(targetTerritory->getArmies()) + " troops");

            } else { //the player successfully defended his own territory
                int result = targetTerritory->getArmies()*0.7 - numArmy*0.6;
                sourceTerritory->setArmies(sourceTerritory->getArmies()-numArmy);
                targetTerritory->setArmies(result);
                executed = true;
                setExecutionEffect("Unsuccessfully advanced, you lost " + to_string(numArmy)   + " troops; " + sourceTerritory->getName() + " has now " + to_string(sourceTerritory->getArmies()) + " troops and "  + targetTerritory->getName() + " has now " + to_string(targetTerritory->getArmies()) + " troops" );

            }
        }
    }
}

Order* Advance::clone() const {
    return new Advance(*this);
}

std::string Advance::getName() const{
    return "Advance";
}

//BOMB CLASS DEFINITION---------------------------------------------------

Bomb::Bomb(Player* p, Territory *wantedTerritory, Territory *sTerritory) {
    player=p;
    targetTerritory = wantedTerritory;
    sourceTerritory = sTerritory;
}

Bomb::Bomb(const Bomb &other) : Order(other) {
    targetTerritory = other.targetTerritory;
}

// Assignment operator
Bomb &Bomb::operator=(const Bomb &other) {
    if (this != &other) {
        Order::operator=(other);
        targetTerritory = other.targetTerritory;
    }
    return *this;
}

// Output for Bomb
ostream &operator<<(ostream &os, const Bomb& b) {
    os << "Bomb " << b.targetTerritory;
    return os;
}

// Validate bombing conditions
bool Bomb::validate() {
    return player != targetTerritory->getPlayer() &&  player->getHand()->includes(this->getName()) && targetTerritory->isEdge(sourceTerritory);
}

void Bomb::execute() {
    if (validate()) {
        targetTerritory->setArmies(targetTerritory->getArmies()/2);
        executed = true;
        setExecutionEffect("Successfully bombed " + targetTerritory->getName());
    }
}

Order* Bomb::clone() const {
    return new Bomb(*this);
}

std::string Bomb::getName() const{
    return "Bomb";
}

//BLOCKADE CLASS DEFINTION---------------------------------------------------

// Constructor for Blockade
Blockade::Blockade(Player* p, Territory *wantedTerritory) {
    player=p;
    targetTerritory = wantedTerritory;
}

// Copy constructor
Blockade::Blockade(const Blockade &other) : Order(other) {
    targetTerritory = other.targetTerritory;
}

// Assignment operator
Blockade &Blockade::operator=(const Blockade &other) {
    if (this != &other) {
        Order::operator=(other);
        targetTerritory = other.targetTerritory;
    }
    return *this;
}

// Output for Blockade
ostream &operator<<(ostream &os, const Blockade& b) {
    os << "Blockade " << b.targetTerritory;
    return os;
}

// Validate blockade conditions
bool Blockade::validate() {
    return player == targetTerritory->getPlayer() && player->getHand()->includes(this->getName());
}

void Blockade::execute() {
    if (validate()) {
        targetTerritory->setArmies(targetTerritory->getArmies()*3);
        //make it neutral territory
        executed = true;
        setExecutionEffect("Successfully blockade " + targetTerritory->getName());
    }
}

Order* Blockade::clone() const {
    return new Blockade(*this);
}

std::string Blockade::getName() const{
    return "Blockade";
}

//AIRLIFT CLASS DEFINITION---------------------------------------------------

// Constructor for Airlift
Airlift::Airlift(Player* p, int nArmy, Territory *sTerritory, Territory *tTerritory) {
    player=p;
    numArmy = nArmy;
    sourceTerritory = sTerritory;
    targetTerritory = tTerritory;
}

// Copy constructor
Airlift::Airlift(const Airlift &other) : Order(other) {
    numArmy = other.numArmy;
    targetTerritory = other.targetTerritory;
}

// Assignment operator
Airlift& Airlift::operator=(const Airlift& other) {
    if (this != &other) {
        Order::operator=(other);
        numArmy = other.numArmy;
        targetTerritory = other.targetTerritory;
        sourceTerritory = other.sourceTerritory;
    }
    return *this;
}

// Output for Airlift
ostream &operator<<(ostream &os, const Airlift& a) {
    os << "Airlift " << a.targetTerritory;
    return os;
}

// Validate airlift conditions
bool Airlift::validate() {
    return player == targetTerritory->getPlayer() && sourceTerritory->getArmies() > numArmy && numArmy > 0 && player->getHand()->includes(this->getName());
}

void Airlift::execute() {
    if (validate()) {
        sourceTerritory->setArmies(sourceTerritory->getArmies()-numArmy);
        targetTerritory->setArmies(sourceTerritory->getArmies()+numArmy);
        executed = true;
        setExecutionEffect("Successfully airlift " + to_string(numArmy) + " troops from " + sourceTerritory->getName() + " to " + targetTerritory->getName());
    }
}

Order* Airlift::clone() const {
    return new Airlift(*this);
}

std::string Airlift::getName() const{
    return "Airlift";
}

//NEGOTIATE CLASS DEFINITION---------------------------------------------------

// Constructor for Negotiate
Negotiate::Negotiate(Player *p, Player *tPlayer) {
    player=p;
    targetPlayer = tPlayer;
}

// Copy constructor
Negotiate::Negotiate(const Negotiate &other) : Order(other) {
    player= other.player;
    targetPlayer = other.targetPlayer;
}

// Assignment operator
Negotiate &Negotiate::operator=(const Negotiate &other) {
    if (this != &other) {
        Order::operator=(other);
        player= other.player;
        targetPlayer = other.targetPlayer;
    }
    return *this;
}

// Output for Negotiate
ostream &operator<<(ostream &os, const Negotiate& n) {
    os << "Negotiate " << n.targetPlayer;
    return os;
}

// Validate negotiation
bool Negotiate::validate() {
    return player != targetPlayer && player->getHand()->includes(this->getName());
}

void Negotiate::execute() {
    if (validate()) {
        //MAKE SURE NO PLAYER CAN CALL THE ADVANCE ORDER ON THE OTHER
        executed = true;
        setExecutionEffect("Successfully negotiate with " + targetPlayer->getName() + ". You cannot call the advance order for the next round");
    }
}

Order* Negotiate::clone() const {
    return new Negotiate(*this);
}

std::string Negotiate::getName() const{
    return "Negotiate";
}

//ORDERSLIST CLASS DEFINITION---------------------------------------------------

// Copy constructor (deep copy using clone)
OrdersList::OrdersList(const OrdersList& other) {
    for (const auto& order : other.orders) {
        orders.push_back(order->clone());
    }
}

// Assignment operator
OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        orders.clear();
        for (const auto& order : other.orders) {
            orders.push_back(order->clone());
        }
    }
    return *this;
}

OrdersList::~OrdersList() {
    for (auto* order : orders) {
        delete order;
    }
    orders.clear();
}

ostream &operator<<(ostream &os, const OrdersList& list) {
    for (const auto& order : list.orders) {
        os << order->getPlayer() << ": Order -> " << order->getName() << endl;
    }
    return os;
}

void OrdersList::addOrder(Order* order) {
    if (order) {
        orders.push_back(order); // Deleted std::move
    }
}

// Remove an order by index
void OrdersList::remove(int index) {
    if (index >= 0 && index < static_cast<int>(orders.size())) {
        orders.erase(orders.begin() + index);
    }
}

// Move an order from one index to another
void OrdersList::move(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < static_cast<int>(orders.size()) &&
        toIndex >= 0 && toIndex < static_cast<int>(orders.size())) {

        auto order = std::move(orders[fromIndex]);
        orders.erase(orders.begin() + fromIndex);
        orders.insert(orders.begin() + toIndex, std::move(order));
        }
}

// Return number of orders in the list
size_t OrdersList::size() const {
    return orders.size();
}

// Retrieve a pointer to an order by index
Order* OrdersList::getOrder(int index) const {
    if (index >= 0 && index < static_cast<int>(orders.size())) {
        //return orders[index].get();
        return orders[index];
    }
    return nullptr;
}
