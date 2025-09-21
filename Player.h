#ifndef player_h
#define player_h

#include <vector>
#include <string>
#include "Orders.h"
#include "Map.h"
#include "Cards.h"


class Player {
public:
	// Function to return a list of territories the player can attack
	std::vector<Territory*> toAttack();

	// Function to return a list of territories the player can defend  
	std::vector<Territory*> toDefend();

	// Issue an order based on the order type provided
	void issueOrder(string* orderType);

private:
	Hand* hand;
	OrderList* orderList;

};

#endif /* player_h */