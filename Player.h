#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <array>

using namespace std;

// Forward declarations to avoid circular dependencies
class Territory;
class Hand;
class Order;
class OrdersList;
class Card;
class Deck;
class Continent;
class Deploy;

typedef shared_ptr<Card> SpCard;   // Type alias
typedef shared_ptr<Hand> SpHand;   // Type alias
typedef shared_ptr<Order> SpOrder; // Type alias

////////////////////////////////////////////////////////////////////////////////////

// This enum is used in conjunction with issueOrderStatus to track which phases of order issuing have been completed.
enum IssuePhase
{
	AttackDefendPhase = 0,
	DeployPhase = 1,
	AdvancePhase = 2,
	OtherPhase = 3
};

// Main Player class
class Player
{
public:
	// Constructors
	Player();				 // default
	Player(string name);	 // parameterized (name of player)
	Player(const Player &p); // copy constructor
	~Player();

	// stream insertion operators
	friend ostream &operator<<(ostream &out, const Player &player);
	Player &operator=(const Player &p);

	// Function to return a list of territories the player can attack
	std::vector<Territory *> toAttack();

	// Function to return a list of territories the player can defend
	std::vector<Territory *> toDefend();

	// Issue an order based on the order type provided
	void issueOrder();

	// Getters setters
	void setTerritories(vector<Territory *> *terrs); // this will be used to assign territories to the player at the start of the game
	vector<Territory *> *getTerritories() const;

	void setHand(Hand *h);
	Hand *getHand() const;

	void setOrdersList(OrdersList *ol);
	OrdersList *getOrdersList() const;

	void setName(string name);
	string getName() const;

	void setReinforcementPool(int num);
	int getReinforcementPool() const;

	void displayTerritories(const std::vector<Territory *> &territories);
	void displayOrdersList(const OrdersList *ordersList);
	void displayHand(const Hand *hand);

private:
	// Attributes
	string *name;					  // Player's name
	Hand *hand;						  // Player's hand of cards
	OrdersList *ordersList;			  // List of orders of the player
	vector<Territory *> *territories; // Collection of pointers to territories owned by the player
	vector<Territory *> *attacking;	  // Collection of pointers to territories the player is attacking
	vector<Territory *> *defending;	  // Collection of pointers to territories the player is defending
	int *reinforcementPool;			  // Number of armies available to the player for deployment
	array<bool, 4> *issueOrderStatus; // Status of issued orders

	// Helper methods to get attackable and defendable territories
	unordered_map<string, Territory *> getAttackableTerritories();
	unordered_map<string, Territory *> getDefendableTerritories();
	Deploy *deploy(vector<Territory *> &defendingTerritories);
	Advance *advance(vector<Territory *> &attackingTerritories, vector<Territory *> &defendingTerritories);
	bool isDoneIssuingOrder();
};

void testPlayers();
void printTerritories(const vector<Territory *> &territories);
void printOrdersList(const OrdersList *ordersList);
