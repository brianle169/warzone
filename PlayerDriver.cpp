#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "Map.h"

using namespace std;

// print the territories in the vector
void printTerritories(const std::vector<Territory*>& territories) {
    for (Territory* t : territories) {              // <- change here
        if (t) {
            std::cout << "Name: "   << t->getName()
                      << " - Armies: " << t->getArmies() << '\n';
        }
    }
}

void printOrdersList(const OrdersList* ordersList) {
    if (!ordersList) {
        cout << "OrdersList size: 0\n";
        return;
    }
    cout << "OrdersList size: " << ordersList->size() << "\n";
}

// testPlayers(): function to test the Player class and its methods
void testPlayers() {
	cout << "=== Testing Player Class ===" << endl;

	cout << "=== General Information === " << endl;
	Player* p1 = new Player("Minh");
	cout << *p1 << endl;

	cout << "=== Copy Constructor ===" << endl;
	Player p2(*p1);
	p2.setName("Anas");
	cout << p2 << endl;

	cout << "=== toAttack() Method ===" << endl;
	cout << "Player " << p1->getName() << " will attack the following territories: " << endl;
	vector<Territory*> attackable = p1->toAttack();
	printTerritories(attackable);
	cout << endl;

	// free memory allocated in toAttack()
	for (Territory* t : attackable) {
		delete t;
	}

	cout << "=== toDefend() Method ===" << endl;
	cout << "Player " << p1->getName() << " will defend the following territories: " << endl;
	vector<Territory*> defending = p1->toDefend();
	printTerritories(defending);
	cout << endl;

	// free memory allocated in toDefend()
	for (Territory* t : defending) {
		delete t;
	}

	cout << "=== issueOrder() Method ===" << endl;
	cout << "Issuing DEPLOY order..." << endl;
	p1->issueOrder(DEPLOY);
	p1->issueOrder(AIRLIFT);
	p1->issueOrder(ADVANCE);
	p1->issueOrder(BLOCKADE);
	p1->issueOrder(BOMB);
	p1->issueOrder(NEGOTIATE);
	printOrdersList(p1->getOrdersList());

	delete p1;
}