#pragma once
#include <iostream>
#include <vector>

class Player;
class Territory;
class Hand;

// Base class for all orders
class Order {
protected:
    std::string orderName; // Name of the order
    Player* player;        // Player issuing the order
    bool executed;         // Execution status
    std::string executionEffect; // Description of execution effect
public:
    Order() = default;
    Order(std::string orderN, Player* player);
    Order(const Order& other);
    Order& operator=(const Order& other);
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    virtual ~Order() = default;
    void setExecutionEffect(const std::string &effect);
    virtual Order* clone() const = 0;
    virtual std::string getName() const = 0;
    virtual bool validate() = 0;
    virtual void execute() = 0;
    std::string getPlayer() const;
};

// Deploy order: deploy armies to a territory
class Deploy : public Order {
private:
    Territory* targetTerritory; // Territory to deploy armies
    int numArmies; // Number of armies to deploy
public:
    Deploy() = default; // default constructor
    Deploy(Player* p, Territory* targetTerritory, int numArmies); // parameterized constructor
    Deploy(const Deploy& other); // copy constructor
    Deploy& operator=(const Deploy& other); // assignment operator
    friend std::ostream& operator<<(std::ostream &strm, const Deploy&); //stream insertion operator
    virtual ~Deploy() = default; // destructor
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
};

// Advance order: move armies from one territory to another
class Advance : public Order {
private:
    int numArmies; // Number of armies to move
    Territory* sourceTerritory; // Source territory
    Territory* targetTerritory; // Target territory
public:
    Advance() = default;
    Advance(Player* p, int moveNumArmy, Territory* baseTerritory, Territory* wantedTerritory);
    Advance(const Advance& other);
    Advance& operator=(const Advance& other);
    friend std::ostream& operator<<(std::ostream &strm, const Advance&);
    virtual ~Advance() = default;
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
};

// Bomb order: bomb a territory to reduce armies
class Bomb : public Order {
private:
    Territory* targetTerritory;
    Territory* sourceTerritory;
public:
    Bomb() = default;
    Bomb(Player *p, Territory* wantedTerritory, Territory* sTerritory);
    Bomb(const Bomb& other);
    Bomb& operator=(const Bomb& other);
    friend std::ostream& operator<<(std::ostream &strm, const Bomb&);
    virtual ~Bomb() = default;
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
};

// Blockade order: place a blockade on a territory
class Blockade : public Order {
private:
    Territory* targetTerritory; // Territory to blockade
public:
    Blockade() = default;
    Blockade(Player* p, Territory* wantedTerritory);
    Blockade(const Blockade& other);
    Blockade& operator=(const Blockade& other);
    friend std::ostream& operator<<(std::ostream &strm, const Blockade&);
    virtual ~Blockade() = default;
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
};

// Airlift order: move armies between territories regardless of adjacency
class Airlift : public Order {
private:
    int numArmy; // Number of armies to airlift
    Territory* sourceTerritory; // Source territory
    Territory* targetTerritory; // Target territory
public:
    Airlift() = default;
    Airlift(Player* p, int nArmy, Territory* sTerritory, Territory* tTerritory);
    Airlift(const Airlift& other);
    Airlift& operator=(const Airlift& other);
    friend std::ostream& operator<<(std::ostream &strm, const Airlift&);
    virtual ~Airlift() = default;
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
};

// Negotiate order: negotiate peace between players
class Negotiate : public Order {
private:
    Player* targetPlayer; // Player to negotiate with
public:
    Negotiate() = default;
    Negotiate(Player* p, Player* tPlayer );
    Negotiate(const Negotiate& other);
    Negotiate& operator=(const Negotiate& other);
    friend std::ostream& operator<<(std::ostream &strm, const Negotiate&);
    virtual ~Negotiate() = default;
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
};

// Container class managing a list of orders
class OrdersList {
private:
    std::vector<Order*> orders;
public:
    OrdersList() = default;
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);
    friend std::ostream& operator<<(std::ostream &strm, const OrdersList&);
    ~OrdersList();

    void addOrder(Order* order);
    void move(int fromIndex, int toIndex);
    void remove(int fromIndex);
    Order* getOrder(int index) const;
    size_t size() const;
};

void testOrdersList(); // Test function for OrdersList class