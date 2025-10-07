#include "Orders.h"
#include <iostream>
using namespace std;

//TEMP CLASSES; Delete player, territory and hand class after assignment1
//PROF GAVE THE OK

// Simple Player constructor that sets name
Player::Player(const std::string &n) {
    name=n;
}

// Simple Territory constructor assigning name, player, and armies
Territory::Territory(const std::string &n, Player *p, int a) {
    name=n;
    player=p;
    armies=a;
}

// Temporary card validation stub
static bool getCard() {
    return false;
}

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
    os << order.player->name << ": " << order.getName();
    if (order.executed) {
        os << " EXECUTED -> " << order.executionEffect;
    }
    return os;
}

// Setter for execution effect description
void Order::setExecutionEffect(const std::string& effect) {
    executionEffect = effect;
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
    os << "Deploy " << d.numArmies << " armies to territory " << d.targetTerritory->name;
    return os;
}

// Validates if Deploy order can execute
bool Deploy::validate() {
    if (targetTerritory == nullptr || player == nullptr || player != targetTerritory->player || numArmies < 0) {
        return false;
    }
    return true;
}

// Executes the Deploy order
void Deploy::execute() {
    if (validate()) {
        targetTerritory->armies += numArmies;
        executed = true;
        setExecutionEffect("Successfully deployed " + to_string(numArmies) + " armies to " + targetTerritory->name + "; " + targetTerritory->name + " has now " + to_string(targetTerritory->armies) + " troops");
    }
}

// Clone method
std::unique_ptr<Order> Deploy::clone() const {
    return std::make_unique<Deploy>(*this);
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
    if (player != sourceTerritory->player || sourceTerritory->armies < numArmy || numArmy < 0 && targetTerritory->isAdjacent()) { //TEMP FUNCTION UNTIL ASSIGNMENT 2 WHERE WE CAN USE OTHER CLASSES AND FILES
        return false;
    }
    return true;
}

// Executes troop movement and combat logic
void Advance::execute() {
    if (validate()) {
        if (player == targetTerritory->player) { //the player wants to advance troops on hiw own territory
            sourceTerritory->armies -= numArmy;
            targetTerritory->armies += numArmy;
            executed = true;
            setExecutionEffect("Successfully advanced " + to_string(numArmy) + " troops from " + sourceTerritory->name + " to " + targetTerritory->name + "; " + targetTerritory->name + " has now " + to_string(targetTerritory->armies) + " troops");
        }

        if (player != targetTerritory->player) { //the player successfully conquered the territory
            if (numArmy*0.6 > targetTerritory->armies*0.7) {
                int result = numArmy*0.6 - targetTerritory->armies*0.7;
                sourceTerritory->armies -= numArmy;
                targetTerritory->armies = result;
                targetTerritory->player = player;
                //add a card to the deck since conquered
                executed = true;
                setExecutionEffect("Successfully conquered and advanced " + to_string(numArmy) + " troops from " + sourceTerritory->name + " to " + targetTerritory->name+ "; "+ sourceTerritory->name + " has now " + to_string(sourceTerritory->armies) + " troops and "  + targetTerritory->name + " has now " + to_string(targetTerritory->armies) + " troops");

            } else { //the player successfully defended his own territory
                int result = targetTerritory->armies*0.7 - numArmy*0.6;
                sourceTerritory->armies -= numArmy;
                targetTerritory->armies = result;
                executed = true;
                setExecutionEffect("Unsuccessfully advanced, you lost " + to_string(numArmy)   + " troops; " + sourceTerritory->name + " has now " + to_string(sourceTerritory->armies) + " troops and "  + targetTerritory->name + " has now " + to_string(targetTerritory->armies) + " troops" );

            }
        }
    }
}

// Clone for Advance
std::unique_ptr<Order> Advance::clone() const {
    return std::make_unique<Advance>(*this);
}

//BOMB CLASS DEFINITION---------------------------------------------------

// Constructor for Bomb
Bomb::Bomb(Player* p, Territory *wantedTerritory) {
    player=p;
    targetTerritory = wantedTerritory;
}

// Copy constructor
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
    if (player == targetTerritory->player &&  getCard() == false && targetTerritory->isAdjacent() ) { //TEMP FUNCTION UNTIL ASSIGNMENT 2 WHERE WE CAN USE OTHER CLASSES AND FILES
        return false;
    }
    return true;
}

// Executes Bomb (halves enemy armies)
void Bomb::execute() {
    if (validate()) {
        targetTerritory->armies = targetTerritory->armies/2;
        executed = true;
        setExecutionEffect("Successfully bombed " + targetTerritory->name);
    }
}

// Clone for Bomb
std::unique_ptr<Order> Bomb::clone() const {
    return std::make_unique<Bomb>(*this);
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
    if (player != targetTerritory->player && getCard() == false) { //TEMP FUNCTION UNTIL ASSIGNMENT 2 WHERE WE CAN USE OTHER CLASSES AND FILES
        return false;
    }
    return true;
}

// Execute blockade (triples armies, neutral territory)
void Blockade::execute() {
    if (validate()) {
        targetTerritory->armies = targetTerritory->armies*3;
        //make it neutral territory
        executed = true;
        setExecutionEffect("Successfully blockade " + targetTerritory->name);

    }
}

// Clone for Blockade
std::unique_ptr<Order> Blockade::clone() const {
    return std::make_unique<Blockade>(*this);
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
    if (player != targetTerritory->player || sourceTerritory->armies < numArmy || numArmy < 0 && getCard() == false) { //TEMP FUNCTION UNTIL ASSIGNMENT 2 WHERE WE CAN USE OTHER CLASSES AND FILES
        return false;
    }
    return true;
}

// Execute airlift (move troops)
void Airlift::execute() {
    if (validate()) {
        sourceTerritory->armies -= numArmy;
        targetTerritory->armies += numArmy;
        executed = true;
        setExecutionEffect("Successfully airlift " + to_string(numArmy) + " troops from " + sourceTerritory->name + " to " + targetTerritory->name);
    }
}

// Clone for Airlift
std::unique_ptr<Order> Airlift::clone() const {
    return std::make_unique<Airlift>(*this);
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
    if (player == targetPlayer && getCard() == false) { //TEMP FUNCTION UNTIL ASSIGNMENT 2 WHERE WE CAN USE OTHER CLASSES AND FILES
        return false;
    }
    return true;
}

// Execute negotiation (no attack next round)
void Negotiate::execute() {
    if (validate()) {
        //MAKE SURE NO PLAYER CAN CALL THE ADVANCE ORDER ON THE OTHER
        executed = true;
        setExecutionEffect("Successfully negotiate with " + targetPlayer->name + ". You cannot call the advance order for the next round");
    }
}

// Clone for Negotiate
std::unique_ptr<Order> Negotiate::clone() const {
    return std::make_unique<Negotiate>(*this);
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

// Output for OrdersList
ostream &operator<<(ostream &os, const OrdersList& list) {
    for (const auto& order : list.orders) {
        os << order->getPlayer() << ": Order -> " << order->getName() << endl;
    }
    return os;
}

// Add a new order to the list
void OrdersList::addOrder(std::unique_ptr<Order> order) {
    if (order) {
        orders.push_back(std::move(order));
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
        return orders[index].get();
    }
    return nullptr;
}

//ORDERSLIST TEST FUNCTION DEFINITION---------------------------------------------------

void testOrdersList() {
    Player* player1 = new Player("Anas");
    Player* player2 = new Player("West");

    Territory* territory1 = new Territory("Canada", player1, 100);
    Territory* territory2 = new Territory("USA", player2, 50);
    Territory* territory3 = new Territory("Japan", player1, 300);
    Territory* territory4 = new Territory("Germany", player2, 500);

    cout << "Testing OrdersList" << endl;

    OrdersList orders_list;

    orders_list.addOrder(make_unique<Deploy>(player1,territory1,10)); //working case
    orders_list.addOrder(make_unique<Deploy>(player1,territory3,300)); //working
    orders_list.addOrder(make_unique<Advance>(player1,150,territory1, territory3));//invalid not enough army
    orders_list.addOrder(make_unique<Advance>(player1,150,territory3, territory2)); //conquer
    orders_list.addOrder(make_unique<Advance>(player1,10,territory1, territory2));//defense
    orders_list.addOrder(make_unique<Deploy>(player2,territory1,200)); //not working case
    orders_list.addOrder(make_unique<Bomb>(player1, territory1));//invalid,own territory
    orders_list.addOrder(make_unique<Bomb>(player1, territory4));//valid
    orders_list.addOrder(make_unique<Blockade>(player1, territory4)); //invalid,not his territory
    orders_list.addOrder(make_unique<Blockade>(player1, territory1));// valid
    orders_list.addOrder(make_unique<Airlift>(player1,10,territory1,territory2));//invalid,not his territory
    orders_list.addOrder(make_unique<Airlift>(player1,10,territory1,territory3));//valid
    orders_list.addOrder(make_unique<Negotiate>(player1,player1));//invalid, same player
    orders_list.addOrder(make_unique<Negotiate>(player1,player2));//valid



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

    //Display the size
    cout << orders_list.size() << endl;
    //Get an order
    cout << *orders_list.getOrder(0);

};
