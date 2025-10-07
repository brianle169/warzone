#pragma once
#include <iostream>
#include <vector>

class Player;
class Territory;
class Hand;

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
    virtual ~Order() = default;
    void setExecutionEffect(const std::string &effect);
    virtual Order* clone() const = 0;
    virtual std::string getName() const = 0;
    virtual bool validate() = 0;
    virtual void execute() = 0;
    std::string getPlayer() const;
};

class Deploy : public Order {
private:
    Territory* targetTerritory;
    int numArmies;
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
    virtual ~Advance() = default;
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
};

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

class Blockade : public Order {
private:
    Territory* targetTerritory;
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
    virtual ~Airlift() = default;
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
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
    virtual ~Negotiate() = default;
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    std::string getName() const override;
};

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

void testOrdersList();