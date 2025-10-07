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

//ORDERSLIST TEST FUNCTION DEFINITION---------------------------------------------------

void testOrdersList() {
    Player* player1 = new Player("Anas");
    Player* player2 = new Player("West");

    Territory* territory1 = new Territory("Canada", nullptr, 100);
    Territory* territory2 = new Territory("USA", nullptr, 50);
    Territory* territory3 = new Territory("Japan", nullptr, 300);
    Territory* territory4 = new Territory("Germany", nullptr, 500);
    territory4->addEdge(territory1);
    territory1->addEdge(territory4);
    territory3->addEdge(territory2);
    territory2->addEdge(territory3);
    territory1->addEdge(territory2);
    territory2->addEdge(territory1);

    SpCard bc1 = SpCard(new BombCard());
    SpCard rc1 = SpCard(new ReinforcementCard());
    SpCard blc1 = SpCard(new BlockadeCard());
    SpCard ac1 = SpCard(new AirliftCard());
    SpCard dc1 = SpCard(new DiplomacyCard());

    SpCard bc2 = SpCard(new BombCard());
    SpCard rc2 = SpCard(new ReinforcementCard());
    SpCard blc2 = SpCard(new BlockadeCard());
    SpCard ac2 = SpCard(new AirliftCard());
    SpCard dc2 = SpCard(new DiplomacyCard());

    player1->getHand()->add(bc1);
    player1->getHand()->add(rc1);
    player1->getHand()->add(blc1);
    player1->getHand()->add(ac1);
    player1->getHand()->add(dc1);

    player2->getHand()->add(bc2);
    player2->getHand()->add(rc2);
    player2->getHand()->add(blc2);
    player2->getHand()->add(ac2);
    player2->getHand()->add(dc2);

    territory1->setPlayer(player1);
    territory2->setPlayer(player2);
    territory3->setPlayer(player1);
    territory4->setPlayer(player2);

    cout << "Testing OrdersList" << endl;

    OrdersList orders_list;

    orders_list.addOrder( new Deploy(player1,territory1,10)); //working case
    orders_list.addOrder( new Deploy(player1,territory3,300)); //working
    orders_list.addOrder( new Advance(player1,150,territory1, territory3));//invalid not enough army
    orders_list.addOrder( new Advance(player1,150,territory3, territory2)); //conquer
    orders_list.addOrder( new Advance(player1,10,territory1, territory2)); //defense
    orders_list.addOrder( new Deploy(player2,territory1,200)); //not working case
    orders_list.addOrder( new Blockade(player1, territory4)); //invalid,not his territory
    orders_list.addOrder( new Blockade(player1, territory1));// valid
    orders_list.addOrder( new Bomb(player1, territory1, territory1));//invalid,own territory
    orders_list.addOrder( new Bomb(player1, territory4, territory1));//valid
    orders_list.addOrder( new Airlift(player1,10,territory1,territory2));//invalid,not his territory
    orders_list.addOrder( new Airlift(player1,10,territory1,territory3));//valid
    orders_list.addOrder( new Negotiate(player1,player1));//invalid, same player
    orders_list.addOrder( new Negotiate(player1,player2));//valid



    cout << orders_list << endl;

    cout << "\n2. Testing order validation..." << endl;
    for (int i = 0; i < static_cast<int>(orders_list.size()); ++i) {
        Order* order = orders_list.getOrder(i);
        if (order->validate()) {
            cout << order->getPlayer() << ": " << order->getName() << " STATE -> " << "Valid" << endl;
        } else {
            cout << order->getPlayer() << ": " << order->getName() << " STATE -> " << "Invalid" << endl;
        }
    }

    cout << "\n3. Testing order execution..." << endl;
    for (int i = 0; i < static_cast<int>(orders_list.size()); ++i) {
        Order* order = orders_list.getOrder(i);
        if (order->validate()) {
            order->execute();
            cout << *order << endl;

        }
    }

    cout << "\n4. Testing move operation..." << endl;
    orders_list.move(5,0);
    orders_list.move(13,1);
    cout << orders_list << endl;


    cout << "\n5. Testing remove operation..." << endl;
    orders_list.remove(0);
    orders_list.remove(0);
    orders_list.remove(0);
    orders_list.remove(0);
    orders_list.remove(9);
    cout << orders_list << endl;

    delete player1; 
    delete player2;
    delete territory1;  
    delete territory2;  
    delete territory3; 
    delete territory4;
};
