#ifndef player_h
#define player_h

#include <vector>
#include <string>
#include <iostream>
#include <memory>

using namespace std;

class Territory;
class Hand;
class Order;
class OrdersList;
class Card;
class Deck;
class Continent;
typedef shared_ptr < Card > SpCard; // Type alias 
typedef shared_ptr < Hand > SpHand; // Type alias 
typedef shared_ptr < Order > SpOrder; // Type alias 

////////////////////////////////////////////////////////////////////////////////////
// Enum for different order types. We will map user input on console with these enum options later
enum ORDER_TYPE
{
	ADVANCE,
	AIRLIFT,
	BOMB,
	BLOCKADE,
	DEPLOY,
	NEGOTIATE
};

// Main Player class
class Player {
public:
	// Constructors
	Player(); // default
	Player(string* name); // parameterized (name of player)
	Player(const Player& p); // copy constructor
	~Player();

	// stream insertion operators
	friend ostream& operator<< (ostream& out, const Player& player);
	Player& operator= (const Player& p);

	// Function to return a list of territories the player can attack
	std::vector<Territory*> toAttack();

	// Function to return a list of territories the player can defend  
	std::vector<Territory*> toDefend();

	// Issue an order based on the order type provided
	void issueOrder(ORDER_TYPE otype);

	// Getters setters
	void setTerritories(vector<Territory*>* terrs); // this will be used to assign territories to the player at the start of the game
	vector<Territory*>* getTerritories() const;

	void setHand(Hand* h);
	Hand* getHand() const;

	void setOrdersList(OrdersList* ol);
	OrdersList* getOrdersList() const;

	void setName(string* name);
	string getName() const;


private:
	string* name; // Player's name
	Hand* hand; // Player's hand of cards
	OrdersList* ordersList; // List of orders of the player
	vector<Territory*>* territories; // Collection of pointers to territories owned by the player
};

void testPlayers();
void printTerritories(const vector<Territory*>& territories);
void printOrdersList(const OrdersList* ordersList);

////////////////////////////////////////////////////////////////////////////
// Temp classes
class Hand {
private:
	vector< SpCard > spCards; // all cards in the deck
public:
	// Constructors 
	Hand();
	//Hand(const Hand& other);
	~Hand();
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
	virtual ~Order() = default;
	virtual std::string getName() const = 0;
	std::string getPlayer() const { return player->getName(); }
};

class Deploy : public Order {
private:
	Territory* targetTerritory;
	int numArmies;
public:
	Deploy() = default;                                   // default constructor
	Deploy(Player* p, Territory* targetTerritory, int numArmies); // parameterized constructor
	virtual ~Deploy() = default;                                 // destructor
	std::string getName() const override { return "Deploy"; }
};

class Advance : public Order {
private:
	int numArmy;
	Territory* sourceTerritory;
	Territory* targetTerritory;
public:
	Advance() = default;
	Advance(Player* p, int moveNumArmy, Territory* baseTerritory, Territory* wantedTerritory);
	virtual ~Advance() = default;
	std::string getName() const override { return "Advance"; }
};

class Bomb : public Order {
private:
	Territory* targetTerritory;
public:
	Bomb() = default;
	Bomb(Player* p, Territory* wantedTerritory);
	virtual ~Bomb() = default;
	std::string getName() const override { return "Bomb"; }
};

class Blockade : public Order {
private:
	Territory* targetTerritory;
public:
	Blockade() = default;
	Blockade(Player* p, Territory* wantedTerritory);
	virtual ~Blockade() = default;
	std::string getName() const override { return "Blockade"; }
};

class Airlift : public Order {
private:
	int numArmy;
	Territory* sourceTerritory;
	Territory* targetTerritory;
public:
	Airlift() = default;
	Airlift(Player* p, int nArmy, Territory* sTerritory, Territory* tTerritory);
	virtual ~Airlift() = default;
	std::string getName() const override { return "Airlift"; }
};

class Negotiate : public Order {
private:
	Player* targetPlayer;
public:
	Negotiate() = default;
	Negotiate(Player* p, Player* tPlayer);
	virtual ~Negotiate() = default;
	std::string getName() const override { return "Negotiate"; }
};


class OrdersList {
private:
	//std::vector<std::unique_ptr<Order>> orders; // If use unique ptr, we cannot do copy constructor and assignment operator
	std::vector<Order*> orders; // non unique ptr for simplicity
public:
	OrdersList() = default;
	~OrdersList() = default;

	void addOrder(Order* order);
	Order* getOrder(int index) const;
	size_t size() const;

};


class Territory {
public:

	Territory(const std::string& name, Continent* continent, int armies);

	void addEdge(Territory* edge);

	std::string getName();

	int getArmies();


private:
	std::vector<Territory*> edges;
	std::string name;
	Continent* continent;
	int armies;
	Player* player;
};

class Card {
public:
	// Virtual enables polymorphism, = 0 makes it abstract 
	virtual ~Card(); // Destructor 
};

#endif /* player_h */