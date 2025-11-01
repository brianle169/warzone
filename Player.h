#ifndef PLAYER_H
#define PLAYER_H

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
	Player(string name); // parameterized (name of player)
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

	void setName(string name);
	string getName() const;


private:
	string* name; // Player's name
	Hand* hand; // Player's hand of cards
	OrdersList* ordersList; // List of orders of the player
	vector<Territory*>* territories; // Collection of pointers to territories owned by the player
	int* reinforcementPool; // Number of armies available to the player for deployment
};

void testPlayers();
void printTerritories(const vector<Territory*>& territories);
void printOrdersList(const OrdersList* ordersList);

#endif // PLAYER_H