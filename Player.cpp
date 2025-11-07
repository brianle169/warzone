#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "Map.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////

// Player class implementation
// Default constructor: This constructor instantiate an object with default values:
// -  name is an empty string
// -  hand is a new empty Hand object
// -  ordersList is a new empty OrdersList object
// -  territories is an empty vector of Territory pointers, which will be assigned later by the game engine
Player::Player(): name(new string("")), hand(new Hand()), ordersList(new OrdersList()), reinforcementPool(new int(50)),
territories(new vector<Territory*>()), attacking(new vector<Territory*>()), defending(new vector<Territory*>()) {
}

// Parameterized constructor: This constructor takes a string pointer as a parameter to set the player's name.
// The other attributes are initialized similarly to the default constructor.
Player::Player(string name): hand(new Hand()), ordersList(new OrdersList()), reinforcementPool(new int(50)),
territories(new vector<Territory*>()), attacking(new vector<Territory*>()), defending(new vector<Territory*>()) {
	this->name = new string(name); // This is a pointer assignment.
}

// Copy constructor: This constructor creates a new Player object as a deep copy of an existing Player object.
// It allocates new memory for the name, hand, ordersList, and territories to ensure that the new object has its own copies of these attributes.
// If the source player's territories is null, it initializes territories with default values for testing.
Player::Player(const Player& p) {
	this->name = new string(*(p.name)); // Deep copy
	this->hand = new Hand(*(p.hand)); // use Hand's copy constructor
	this->ordersList = new OrdersList(*(p.ordersList)); // use OrdersList's copy constructor
	this->reinforcementPool = new int(*(p.reinforcementPool)); // Deep copy
	if (p.territories != nullptr) {
		this->territories = new vector<Territory*>(*(p.territories)); // Deep copy of vector
	}
	else {
		this->territories = new vector<Territory*>();
	}
	this->attacking = new vector<Territory*>();
	this->defending = new vector<Territory*>();
}

// Destructor: This destructor releases the memory allocated for the name, hand, ordersList, and territories.
// It also iterates through the territories vector to delete each Territory pointer to prevent memory leaks.
// Finally the territories vector pointer itself is deallocated.
Player::~Player() {
	cout << "Player " << *this->name << " destroyed." << std::endl;
	delete this->name;
	delete this->hand;
	delete this->ordersList;
	// for (Territory* t : *this->territories) {
	// 	delete t; // Free each Territory pointer
	// }
	delete this->reinforcementPool;
	delete this->territories;
}

// Below are the getters and setters for each attribute of the Player class.
// Setters ensure that the existing memory is deallocated before assigning new values.
void Player::setTerritories(vector<Territory*>* terrs) {
	for (Territory* t : *this->territories) {
		delete t; // Free each Territory pointer
	}
	this->territories = terrs;
}

vector<Territory*>* Player::getTerritories() const {
	return this->territories;
}

void Player::setHand(Hand* h) {
	if (this->hand != h) {
		delete this->hand;
		this->hand = h;
	}
}

Hand* Player::getHand() const {
	return this->hand;
}

void Player::setOrdersList(OrdersList* ol) {
	if (this->ordersList != ol) {
		delete this->ordersList;
		this->ordersList = ol;
	}
}

OrdersList* Player::getOrdersList() const {
	return this->ordersList;
}

void Player::setName(string name) {
	delete this->name;
	this->name =new string(name);
}

string Player::getName() const {
	return *(this->name);
}

void Player::setReinforcementPool(int num){
	*(this->reinforcementPool) = num;
}

int* Player::getReinforcementPool() const {
	return this->reinforcementPool;
}

// Stream insertion operator overloading: This operator prints out general information about the Player object.
// Currently for testing, the Player's Hand object is empty, and OrdersList is also initially empty, so to check
// if they exist, we only check whether the pointers are pointing to nullptr or not.
// However, the territories vector is printed out in detail, showing each territory's name and number of armies.
ostream& operator<< (ostream& out, const Player& player) {
	out << "Player Name: " << *(player.name) << endl;
	out << "Player's Hand exists? " << (player.hand != nullptr ? "Yes" : "No") << endl;
	out << "Player's OrdersList exists? " << (player.ordersList != nullptr ? "Yes" : "No") << endl;
	out << "Player's Territories exists? " << (player.territories != nullptr ? "Yes" : "No") << endl;
	for (Territory* t : *player.territories) {
		out << "Name: " << t->getName() << " - Armies: " << t->getArmies() << endl;
	}
	return out; // out = output stream
}

// Assignment operator overloading: This operator assigns the values from one Player object to another.
// It first checks for self-assignment, then deallocates existing memory before performing deep copies of the attributes.
Player& Player::operator= (const Player& p) {
	// If not comparing to itself
	if (this != &p) {
		// Clear the members of this object
		delete this->name;
		delete this->hand;
		delete this->ordersList;
		for (Territory* t : *this->territories) {
			delete t; // Free each Territory pointer
		}

		this->name = new string(*(p.name)); // Deep copy
		this->hand = new Hand(*(p.hand)); // use Hand's copy constructor
		this->ordersList = new OrdersList(*(p.ordersList)); // use OrdersList's copy constructor
		this->territories = p.getTerritories();
	}

	return *this;
}

// getAttackableTerritories() method: return a list of territories that the player can attack.
// these territories are adjacent to the player's own territories and are not owned by the player.
unordered_map<string, Territory*> Player::getAttackableTerritories(){
	unordered_map<string, Territory*> attackableTerritories;
	for (Territory* ownedTerritory : *this->territories) {
		for (Territory* adjacentTerritory : ownedTerritory->getEdges()) {
			string adjacentPName = adjacentTerritory->getPlayer() ? adjacentTerritory->getPlayer()->getName() : "";
			if (adjacentPName != *this->name && attackableTerritories.find(adjacentPName) == attackableTerritories.end()) {
				attackableTerritories[adjacentPName] = adjacentTerritory;
			}
		}
	}
	return attackableTerritories;
}

unordered_map<string, Territory*> Player::getDefendableTerritories(){
	unordered_map<string, Territory*> defendableTerritories;
	for (Territory* ownedTerritory : *this->territories) {
		defendableTerritories[ownedTerritory->getName()] = ownedTerritory;
	}
	return defendableTerritories;
}

// toAttack() method: this method returns a list (vector) of Territory objects which represents the
// territories that the player will attack. In Assignment 1, we only create arbitrary terrtories for testing purposes.
// In assignment 2, we will use the actual territories loaded and assigned to the player at the start of the game.
vector<Territory*> Player::toAttack() {
	// Display current list of attacking territories
	cout << "Attacking List: ";
	for (Territory* t : *this->attacking) {
		cout << t->getName() << ", ";
	}
	cout << endl;

	cout << "Available territories to attack (*: already chosen): ";
	unordered_map<string, Territory*> attackableTerritories = this->getAttackableTerritories();
	for (const auto& pair : attackableTerritories) {
		if (std::find(this->attacking->begin(), this->attacking->end(), pair.second) != this->attacking->end()) {
			cout << pair.first << "*" << " (" << pair.second->getArmies() << ")";
		}
		else {
			cout << pair.first << " (" << pair.second->getArmies() << ")";
		}
		if (pair.second != this->attacking->back()) {
			cout << ", ";
		}
	}
	cout << endl;

	cout << "Select territories to attack (separate by spaces, end with 'x'): ";
	string input;
	getline(cin, input);
	istringstream iss(input);
	string territoryName;

	while (iss >> territoryName) {
		if (territoryName == "x") {
			break;
		}
		auto entry = attackableTerritories.find(territoryName);
		if (entry != attackableTerritories.end() &&  
			std::find(this->attacking->begin(), this->attacking->end(), entry->second) == this->attacking->end()) {
			this->attacking->push_back(entry->second); // insert the territory pointer into attacking vector
		}	
	}

	return *this->attacking;
}

// toDefend() method: the idea is exactly the same with toAttack() method above.
vector<Territory*> Player::toDefend() {
	// Implementation
	cout << "Defending List: ";
	for (Territory* t : *this->defending) {
		cout << t->getName() << ", ";
	}
	cout << endl;

	cout << "Available territories to defend (*: already chosen): ";
	unordered_map<string, Territory*> defendableTerritories = this->getDefendableTerritories();
	for (const auto& pair : defendableTerritories) {
		if (std::find(this->defending->begin(), this->defending->end(), pair.second) != this->defending->end()) {
			cout << pair.first << "*" << " (" << pair.second->getArmies() << ")";
		}
		else {
			cout << pair.first << " (" << pair.second->getArmies() << ")";
		}
		if (pair.second != this->defending->back()) {
			cout << ", ";
		}
	}
	cout << endl;

	cout << "Select territories to defend (separate by spaces, end with 'x'): ";
	string input;
	getline(cin, input);
	istringstream iss(input);
	string territoryName;
	// Arbitrary assignment of territories for testing

	while (iss >> territoryName) {
		if (territoryName == "x") {
			break;
		}
		auto entry = defendableTerritories.find(territoryName);
		if (entry != defendableTerritories.end() && 
			std::find(this->defending->begin(), this->defending->end(), entry->second) == this->defending->end()) {
			this->defending->push_back(entry->second); // insert the territory pointer into defending vector
		}
	}

	return *this->defending;
}

void Player::displayTerritories(const std::vector<Territory*>& territories) {
    for (Territory* t : territories) {           
        if (t) {
            std::cout << t->getName()
                      << " - " << t->getArmies() << '\n';
        }
    }
}

// print the orders list
void Player::displayOrdersList(const OrdersList* ordersList) {
    if (!ordersList) {
        cout << "OrdersList size: 0\n";
        return;
    } else {
		cout << "OrdersList contains: " << endl;
		for(int i = 0; i < static_cast<int>(ordersList->size()); i++){
			if(ordersList->getOrder(i)) {
				cout << " - " << ordersList->getOrder(i)->getName() << endl;
			}
		}
	}
}


void Player::issueOrder() {
	// 1. Print the essential info: name, reinforcement pool, territories, hand, orders list
	cout << "=== Player " << *(this->name) << "'s turn ===" << endl;
	cout << "Reinforcement Pool: " << *(this->reinforcementPool) << endl;
	cout << "Territories (name - armies): " << endl;
	this->displayTerritories(*(this->territories));
	cout << "Hand: " << endl;
	cout << *(this->hand) << endl;
	cout << "Orders List: " << endl;
	this->displayOrdersList(this->ordersList);
	cout << "==========================" << endl;

	// 2. Decide territories to attack and defend.
	vector<Territory*> attackingTerritories = this->toAttack();
	vector<Territory*> defendingTerritories = this->toDefend();
}

void Player::addNegotiatedPlayers(Player* p) {
	negotiatedPlayers.push_back(p);
}

bool Player::isNegotiatedWith(Player* p) const {
    return std::find(negotiatedPlayers.begin(), negotiatedPlayers.end(), p) != negotiatedPlayers.end();
}

Player* Player::neutralPlayer = new Player("Neutral");

Player* Player::getNeutralPlayer() {
    return neutralPlayer;
}
