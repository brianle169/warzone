#include "Orders.h"

#include <iostream>

using namespace std;

//4. All classes must implement a correct copy constructor, assignment operator, and stream insertion operator.
//1. All data members of user-defined class type must be of pointer type.

Player::Player(const std::string &n) {
    name=n;
}
Territory::Territory(const std::string &n, Player *p, int a) {
    name=n;
    player=p;
    armies=a;
}

////////////////////////////////////////////////////////////////////////////

Order::Order(std::string orderN, Player *player) {
    orderName=orderN;
    player= new Player(*player);
}
Order::Order(const Order &other) {
    orderName=other.orderName;
    player=new Player(*other.player);
}

Order& Order::operator=(const Order &other) {
    if (this != &other) {
        orderName=other.orderName;
        player= new Player(*other.player);
    }
    return *this;
}
ostream& operator<<(ostream& os, const Order& order) {
    os << "Order " << order.orderName;
    if (order.executed) {
        os << " [EXECUTED] - " << order.executionEffect;
    }
    return os;
}
Order::~Order() {
    delete player;
}
void Order::setExecutionEffect(const std::string& effect) {
    executionEffect = effect;
}


////////////////////////////////////////////////////////////////////////////

Deploy::Deploy(Player* p,Territory* targetT, int numA) {
    player=new Player(*p);
    targetTerritory = new Territory(*targetT);
    numArmies=numA;
}

Deploy::Deploy(const Deploy& other) : Order(other) {
    numArmies = other.numArmies;
    targetTerritory = new Territory(*other.targetTerritory);
}

Deploy& Deploy::operator=(const Deploy& other) {
    if (this != &other) {
        Order::operator=(other);
        numArmies = other.numArmies;
        targetTerritory = new Territory(*other.targetTerritory);
    }
    return *this;
}

ostream& operator<<(ostream& os, const Deploy& d) {
    os << "Deploy " << d.numArmies << " armies to territory " << d.targetTerritory;
    return os;
}
Deploy::~Deploy() {
    delete targetTerritory;
}

bool Deploy::validate() {
    if (targetTerritory == nullptr || player == nullptr || player != targetTerritory->player || numArmies < 0) {
        return false;
    }
    return true;
}

void Deploy::execute() {
    executed = true;
    if (validate()) {
        targetTerritory->armies += numArmies;
        executed = true;
        setExecutionEffect("Successfully deployed " + std::to_string(numArmies) + " armies to " + targetTerritory->name);
    }
}
////////////////////////////////////////////////////////////////////////////

Advance::Advance(Player* p, int moveNumArmy, Territory *baseTerritory, Territory *wantedTerritory) {
    player= new Player(*p);
    numArmy = moveNumArmy;
    sourceTerritory = new Territory(*baseTerritory);
    targetTerritory = new Territory(*wantedTerritory);
}
Advance::Advance(const Advance &other) : Order(other) {
    numArmy = other.numArmy;
    sourceTerritory = new Territory(*other.sourceTerritory);
    targetTerritory = new Territory(*other.targetTerritory);
}
Advance& Advance::operator=(const Advance &other) {
    if (this != &other) {
        Order::operator=(other);
        numArmy = other.numArmy;
        sourceTerritory = new Territory(*other.sourceTerritory);
        targetTerritory = new Territory(*other.targetTerritory);
    }
    return *this;
}
ostream& operator<<(ostream& os, const Advance& a) {
    os << "Advance " << a.numArmy << " from " << a.sourceTerritory << " to " << a.targetTerritory;
    return os;
}
Advance::~Advance() {
    delete sourceTerritory;
    delete targetTerritory;
}
bool Advance::validate() {
    if (player != sourceTerritory->player || sourceTerritory->armies < 0 || numArmy < 0) { //ADD A CONDITION IF THE NEXT TERRITORY IS NOT ADJACENT
        return false;
    }
    return true;
}

void Advance::execute() {
    if (validate()) {
        if (player == targetTerritory->player) { //belongs to the player
            sourceTerritory->armies -= numArmy;
            targetTerritory->armies += numArmy;
        }

        if (player != targetTerritory->player) {
            if (numArmy*0.6 > targetTerritory->armies*0.7) { //attack mechanism
                int result = numArmy*0.6 - targetTerritory->armies*0.7;
                sourceTerritory->armies -= numArmy;
                targetTerritory->armies = result;
                targetTerritory->player = player;
                //ADD a card to the deck since conquered
            } else { //successfully defended
                int result = targetTerritory->armies*0.7 - numArmy*0.6;
                sourceTerritory->armies -= numArmy;
                targetTerritory->armies = result;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////

Bomb::Bomb(Player* p, Territory *wantedTerritory) {
    player= new Player(*p);
    targetTerritory = new Territory(*wantedTerritory);
}
Bomb::Bomb(const Bomb &other) : Order(other) {
    targetTerritory = new Territory(*other.targetTerritory);
}
Bomb &Bomb::operator=(const Bomb &other) {
    if (this != &other) {
        Order::operator=(other);
        targetTerritory = new Territory(*other.targetTerritory);
    }
    return *this;
}
ostream &operator<<(ostream &os, const Bomb& b) {
    os << "Bomb " << b.targetTerritory;
    return os;
}
Bomb::~Bomb() {
    delete targetTerritory;
}
bool Bomb::validate() {
    if (player == targetTerritory->player ) { //Not bomb your own territory, Not have the bomb card in your hand, ADD A CONDITION IF THE NEXT TERRITORY IS NOT ADJACENT
        return false;
    }
    return true;
}
void Bomb::execute() {
    if (validate()) {
        targetTerritory->armies = targetTerritory->armies/2;
    }
}

////////////////////////////////////////////////////////////////////////////

Blockade::Blockade(Player* p, Territory *wantedTerritory) {
    player= new Player(*p);
    targetTerritory = new Territory(*wantedTerritory);
}
Blockade::Blockade(const Blockade &other) : Order(other) {
    targetTerritory = new Territory(*other.targetTerritory);
}
Blockade &Blockade::operator=(const Blockade &other) {
    if (this != &other) {
        Order::operator=(other);
        targetTerritory = new Territory(*other.targetTerritory);
    }
}
ostream &operator<<(ostream &os, const Blockade& b) {
    os << "Blockade " << b.targetTerritory;
    return os;
}
Blockade::~Blockade() {
    delete targetTerritory;
}
bool Blockade::validate() {
    if (player != targetTerritory->player) { //Not your territory, Not have the blockade card
        return false;
    }
    return true;
}
void Blockade::execute() {
    if (validate()) {
        targetTerritory->armies = targetTerritory->armies*3;
        //make it neutral territory
    }
}

////////////////////////////////////////////////////////////////////////////

Airlift::Airlift(Player* p, int nArmy, Territory *sTerritory, Territory *tTerritory) {
    player= new Player(*p);
    numArmy = nArmy;
    sourceTerritory = new Territory(*sTerritory);
    targetTerritory = new Territory(*tTerritory);
}
Airlift::Airlift(const Airlift &other) : Order(other) {
    numArmy = other.numArmy;
    targetTerritory = new Territory(*other.targetTerritory);
}
Airlift& Airlift::operator=(const Airlift& other) {
    if (this != &other) {
        Order::operator=(other);
        numArmy = other.numArmy;
        sourceTerritory = new Territory(*other.sourceTerritory);
    }
}
ostream &operator<<(ostream &os, const Airlift& a) {
    os << "Airlift " << a.targetTerritory;
    return os;
}
Airlift::~Airlift() {
    delete targetTerritory;
}
bool Airlift::validate() {
    if (player != targetTerritory->player || sourceTerritory->armies < numArmy || numArmy < 0) { //Not have the airlift card
        return false;
    }
    return true;
}
void Airlift::execute() {
    if (validate()) {
        sourceTerritory->armies -= numArmy;
        targetTerritory->armies += numArmy;
    }
}

////////////////////////////////////////////////////////////////////////////

Negotiate::Negotiate(Player *p, Player *tPlayer) {
    player= new Player(*p);
    targetPlayer = new Player(*tPlayer);
}
Negotiate::Negotiate(const Negotiate &other) : Order(other) {
    player= new Player(*other.player);
    targetPlayer = new Player(*other.targetPlayer);
}
Negotiate &Negotiate::operator=(const Negotiate &other) {
    if (this != &other) {
        Order::operator=(other);
        player= new Player(*other.player);
        targetPlayer = new Player(*other.targetPlayer);
    }
}
ostream &operator<<(ostream &os, const Negotiate& n) {
    os << "Negotiate " << n.targetPlayer;
    return os;
}
Negotiate::~Negotiate() {
    delete player;
    delete targetPlayer;
}
bool Negotiate::validate() {
    if (player == targetPlayer) { //Not have the negotiate card
        return false;
    }
}
void Negotiate::execute() {
    if (validate()) {
        //MAKE SURE NO PLAYER CAN CALL THE ADVANCE ORDER ON THE OTHER
    }
}

OrdersList::OrdersList(const OrdersList& other) {
    for (const auto& order : other.orders) {
        //orders.push_back();  // clone each order properly
    }
}

void testOrdersList();

////////////////////////////////////////////////////////////////////////////
