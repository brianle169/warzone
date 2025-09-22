#pragma once
#include <iostream>
#include <vector>
//4. All classes must implement a correct copy constructor, assignment operator, and stream insertion operator.
//1. All data members of user-defined class type must be of pointer type.


class Player {
public:
    std::string name;
    Player(const std::string& n);
};
class Territory {
public:
    std::string name;
    Player* player;
    int armies;
    Territory(const std::string& n, Player* p, int a);
};

class Order {
protected:
    std::string orderName;
    Player* player;
    bool executed;
    std::string executionEffect;
public:
    Order() = default;
    Order(std::string orderN, Player* player);
    Order(const Order& other);
    Order& operator=(const Order& other);
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    ~Order();
    void setExecutionEffect(const std::string &effect);
    std::unique_ptr<Order> clone() const;
};

class Deploy : public Order {
private:
    Territory* targetTerritory;
    int numArmies;
public:
    Deploy() = default;                                   // default constructor
    Deploy(Player* p, Territory* targetTerritory, int numArmies); // parameterized constructor
    Deploy(const Deploy& other);               // copy constructor
    Deploy& operator=(const Deploy& other);   // assignment operator
    friend std::ostream& operator<<(std::ostream &strm, const Deploy&);//stream insertion operator
    ~Deploy();                                 // destructor
    bool validate();
    void execute();
};

class Advance : public Order {
private:
    int numArmy;
    Territory* sourceTerritory;
    Territory* targetTerritory;
public:
    Advance() = default;
    Advance(Player* p, int moveNumArmy, Territory* baseTerritory, Territory* wantedTerritory);
    Advance(const Advance& other);
    Advance& operator=(const Advance& other);
    friend std::ostream& operator<<(std::ostream &strm, const Advance&);
    ~Advance();
    bool validate();
    void execute();
};

class Bomb : public Order {
private:
    Territory* targetTerritory;
public:
    Bomb() = default;
    Bomb(Player *p, Territory* wantedTerritory);
    Bomb(const Bomb& other);
    Bomb& operator=(const Bomb& other);
    friend std::ostream& operator<<(std::ostream &strm, const Bomb&);
    ~Bomb();
    bool validate();
    void execute();
};

class Blockade : public Order {
private:
    Territory* targetTerritory;
public:
    Blockade() = default;
    Blockade(Player* p, Territory* wantedTerritory);
    Blockade(const Blockade& other);
    Blockade& operator=(const Blockade& other);
    friend std::ostream& operator<<(std::ostream &strm, const Blockade&);
    ~Blockade();
    bool validate();
    void execute();
};

class Airlift : public Order {
private:
    int numArmy;
    Territory* sourceTerritory;
    Territory* targetTerritory;
public:
    Airlift() = default;
    Airlift(Player* p, int nArmy, Territory* sTerritory, Territory* tTerritory);
    Airlift(const Airlift& other);
    Airlift& operator=(const Airlift& other);
    friend std::ostream& operator<<(std::ostream &strm, const Airlift&);
    ~Airlift();
    bool validate();
    void execute();
};

class Negotiate : public Order {
private:
    Player* targetPlayer;
public:
    Negotiate() = default;
    Negotiate(Player* p, Player* tPlayer );
    Negotiate(const Negotiate& other);
    Negotiate& operator=(const Negotiate& other);
    friend std::ostream& operator<<(std::ostream &strm, const Negotiate&);
    ~Negotiate();
    bool validate();
    void execute();
};

class OrdersList : public Order {
private:
    std::vector<Order> orders;
public:
    OrdersList() = default;
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);
    friend std::ostream& operator<<(std::ostream &strm, const OrdersList&);
    ~OrdersList();

    void addOrder(std::unique_ptr<Order> order);
    bool move(int fromIndex, int toIndex); //or void
    bool remove(int fromIndex); //or void
    Order* getOrder(int index) const;
    size_t size() const;



};