#ifndef player_h
#define player_h

#include <vector>
#include <string>
#include <iostream>
//#include "Orders.h"
//#include "Map.h"
//#include "Cards.h"

using namespace std;

class Player {
public:
	// Constructors
	Player();
	Player(string* name);

	static int ID; // static variable to keep track of player IDs

	// stream insertion operators
	friend ostream& operator<< (ostream& out, const Player& player);
	friend istream& operator>> (istream& in, const Player& player);

	// Function to return a list of territories the player can attack
	//std::vector<Territory*> toAttack();
	// Function to return a list of territories the player can defend  
	//std::vector<Territory*> toDefend();
	// Issue an order based on the order type provided
	//void issueOrder(string* orderType);

private:
	string* pname; // Player's name
	//Hand* hand; // Player's hand of cards
	//OrderList* orderList; // List of orders of the player
	//std::vector<Territory*> territories; // Territories owned by the player
};

#endif /* player_h */