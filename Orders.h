#pragma once
#include <iostream>
#include <vector>

//TEMP CLASSES; Delete player, territory and hand class after assignment1
//PROF GAVE THE OK
class Player {
public:
    std::string name; // Player's name
    Player(const std::string& n); // Constructor with player name
};
class Territory {
public:
    std::string name; // Territory name
    Player* player;   // Owner of the territory
    int armies;       // Number of armies in the territory
    Territory(const std::string& n, Player* p, int a); // Constructor
    bool isAdjacent() {return true;} // Checks adjacency (stub)
};
class Hand {
public:
    static bool getCard(); // Static method to get a card
};

// Base class for all orders
class Order {
protected:
    std::string orderName; // Name of the order
    Player* player;        // Player issuing the order
    bool executed;         // Execution status
    std::string executionEffect; // Description of execution effect
public:
    Order() = default; // Default constructor
    Order(std::string orderN, Player* player); // Parameterized constructor
    Order(const Order& other); // Copy constructor
    Order& operator=(const Order& other); // Assignment operator
    friend std::ostream& operator<<(std::ostream& os, const Order& order); // Stream output operator
    virtual ~Order() = default; // Virtual destructor
    void setExecutionEffect(const std::string &effect); // Set execution effect description
    virtual std::unique_ptr<Order> clone() const = 0; // Clone the order
    virtual std::string getName() const = 0; // Get order name
    virtual bool validate() = 0; // Validate if the order is legal
    virtual void execute() = 0; // Executes the order
    std::string getPlayer() const { return player->name;} // Get player name
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
    bool validate() override; // Validate deploy order
    void execute() override; // Execute deploy order
    std::unique_ptr<Order> clone() const override; // Clone deploy order
    std::string getName() const override { return "Deploy"; }}; // Get order name

// Advance order: move armies from one territory to another
class Advance : public Order {
private:
    int numArmy; // Number of armies to move
    Territory* sourceTerritory; // Source territory
    Territory* targetTerritory; // Target territory
public:
    Advance() = default; // default constructor
    Advance(Player* p, int moveNumArmy, Territory* baseTerritory, Territory* wantedTerritory); // parameterized constructor
    Advance(const Advance& other); // copy constructor
    Advance& operator=(const Advance& other); // assignment operator
    friend std::ostream& operator<<(std::ostream &strm, const Advance&); // stream insertion operator
    virtual ~Advance() = default; // destructor
    bool validate() override; // Validate advance order
    void execute() override; // Execute advance order
    std::unique_ptr<Order> clone() const override; // Clone advance order
    std::string getName() const override { return "Advance"; } // Get order name
};

// Bomb order: bomb a territory to reduce armies
class Bomb : public Order {
private:
    Territory* targetTerritory; // Territory to bomb
public:
    Bomb() = default; // default constructor
    Bomb(Player *p, Territory* wantedTerritory); // parameterized constructor
    Bomb(const Bomb& other); // copy constructor
    Bomb& operator=(const Bomb& other); // assignment operator
    friend std::ostream& operator<<(std::ostream &strm, const Bomb&); // stream insertion operator
    virtual ~Bomb() = default; // destructor
    bool validate() override; // Validate bomb order
    void execute() override; // Execute bomb order
    std::unique_ptr<Order> clone() const override; // Clone bomb order
    std::string getName() const override { return "Bomb"; } // Get order name
};

// Blockade order: place a blockade on a territory
class Blockade : public Order {
private:
    Territory* targetTerritory; // Territory to blockade
public:
    Blockade() = default; // default constructor
    Blockade(Player* p, Territory* wantedTerritory); // parameterized constructor
    Blockade(const Blockade& other); // copy constructor
    Blockade& operator=(const Blockade& other); // assignment operator
    friend std::ostream& operator<<(std::ostream &strm, const Blockade&); // stream insertion operator
    virtual ~Blockade() = default; // destructor
    bool validate() override; // Validate blockade order
    void execute() override; // Execute blockade order
    std::unique_ptr<Order> clone() const override; // Clone blockade order
    std::string getName() const override { return "Blockade"; } // Get order name
};

// Airlift order: move armies between territories regardless of adjacency
class Airlift : public Order {
private:
    int numArmy; // Number of armies to airlift
    Territory* sourceTerritory; // Source territory
    Territory* targetTerritory; // Target territory
public:
    Airlift() = default; // default constructor
    Airlift(Player* p, int nArmy, Territory* sTerritory, Territory* tTerritory); // parameterized constructor
    Airlift(const Airlift& other); // copy constructor
    Airlift& operator=(const Airlift& other); // assignment operator
    friend std::ostream& operator<<(std::ostream &strm, const Airlift&); // stream insertion operator
    virtual ~Airlift() = default; // destructor
    bool validate() override; // Validate airlift order
    void execute() override; // Execute airlift order
    std::unique_ptr<Order> clone() const override; // Clone airlift order
    std::string getName() const override { return "Airlift"; } // Get order name
};

// Negotiate order: negotiate peace between players
class Negotiate : public Order {
private:
    Player* targetPlayer; // Player to negotiate with
public:
    Negotiate() = default; // default constructor
    Negotiate(Player* p, Player* tPlayer ); // parameterized constructor
    Negotiate(const Negotiate& other); // copy constructor
    Negotiate& operator=(const Negotiate& other); // assignment operator
    friend std::ostream& operator<<(std::ostream &strm, const Negotiate&); // stream insertion operator
    virtual ~Negotiate() = default; // destructor
    bool validate() override; // Validate negotiate order
    void execute() override; // Execute negotiate order
    std::unique_ptr<Order> clone() const override; // Clone negotiate order
    std::string getName() const override { return "Negotiate"; } // Get order name
};

// Container class managing a list of orders
class OrdersList {
private:
    std::vector<std::unique_ptr<Order>> orders; // Vector of orders
public:
    OrdersList() = default; // default constructor
    OrdersList(const OrdersList& other); // copy constructor
    OrdersList& operator=(const OrdersList& other); // assignment operator
    friend std::ostream& operator<<(std::ostream &strm, const OrdersList&); // stream insertion operator
    ~OrdersList() = default; // destructor

    void addOrder(std::unique_ptr<Order> order); // Add order to list
    void move(int fromIndex, int toIndex); // Move order within list
    void remove(int fromIndex); // Remove order from list
    Order* getOrder(int index) const; // Get order at index
    size_t size() const; // Get number of orders
};

void testOrdersList(); // Test function for OrdersList class