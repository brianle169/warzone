#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Player.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////
// Temporary stub classes for testing
Hand::Hand() {
	cout << "Hand created." << endl;
}

Hand::~Hand() {
	// initialize future members?
	cout << "Hand destroyed." << endl;
}

Order::Order(std::string orderN, Player* p) {
	orderName = orderN;
	player = p;
}

void OrdersList::addOrder(Order* order) {
	if (order) {
		orders.push_back(order);
	}
}


size_t OrdersList::size() const {
	return orders.size();
}

Order* OrdersList::getOrder(int index) const {
	if (index >= 0 && index < static_cast<int>(orders.size())) {
		return orders[index];
	}
	return nullptr;
}


Deploy::Deploy(Player* p, Territory* targetT, int numA) {
	player = p;
	targetTerritory = targetT;
	numArmies = numA;
}

Advance::Advance(Player* p, int moveNumArmy, Territory* baseTerritory, Territory* wantedTerritory) {
	player = p;
	numArmy = moveNumArmy;
	sourceTerritory = baseTerritory;
	targetTerritory = wantedTerritory;
}

Bomb::Bomb(Player* p, Territory* wantedTerritory) {
	player = p;
	targetTerritory = wantedTerritory;
}

Blockade::Blockade(Player* p, Territory* wantedTerritory) {
	player = p;
	targetTerritory = wantedTerritory;
}

Airlift::Airlift(Player* p, int nArmy, Territory* sTerritory, Territory* tTerritory) {
	player = p;
	numArmy = nArmy;
	sourceTerritory = sTerritory;
	targetTerritory = tTerritory;
}

Negotiate::Negotiate(Player* p, Player* tPlayer) {
	player = p;
	targetPlayer = tPlayer;
}

Territory::Territory(const std::string& name, Continent* continent, int armies)
	: name(name), continent(continent), armies(armies) {
	//if (this->continent) {
	//	this->continent->addTerritory(this);
	//}
}

void Territory::addEdge(Territory* edge) {
	edges.push_back(edge);
}

std::string Territory::getName() {
	return this->name;
}
int Territory::getArmies() {
	return armies;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

// Player class implementation
// Default constructor: This constructor instantiate an object with default values:
// -  name is an empty string
// -  hand is a new empty Hand object
// -  ordersList is a new empty OrdersList object
// -  territories is an empty vector of Territory pointers, which will be assigned later by the game engine
Player::Player() {
	this->name = new string("");
	this->hand = new Hand();
	this->ordersList = new OrdersList();
	this->territories = new vector<Territory*>();
	territories->push_back(new Territory("DefaultTerritory1", nullptr, 0)); // for testing
	territories->push_back(new Territory("DefaultTerritory2", nullptr, 0)); // for testing
	territories->push_back(new Territory("DefaultTerritory3", nullptr, 0)); // for testing
	territories->push_back(new Territory("DefaultTerritory4", nullptr, 0)); // for testing

}

// Parameterized constructor: This constructor takes a string pointer as a parameter to set the player's name.
// The other attributes are initialized similarly to the default constructor.
Player::Player(string* name) {
	this->name = name; // This is a pointer assignment.
	this->hand = new Hand();
	this->ordersList = new OrdersList();
	//this->territories = nullptr; // initially null, because the game engine will assign territories later
	this->territories = new vector<Territory*>();
	territories->push_back(new Territory("DefaultTerritory1", nullptr, 0)); // for testing
	territories->push_back(new Territory("DefaultTerritory2", nullptr, 0)); // for testing
	territories->push_back(new Territory("DefaultTerritory3", nullptr, 0)); // for testing
	territories->push_back(new Territory("DefaultTerritory4", nullptr, 0)); // for testing

	cout << "Player " << *this->name << " created." << std::endl;
}

// Copy constructor: This constructor creates a new Player object as a deep copy of an existing Player object.
// It allocates new memory for the name, hand, ordersList, and territories to ensure that the new object has its own copies of these attributes.
// If the source player's territories is null, it initializes territories with default values for testing.
Player::Player(const Player& p) {
	this->name = new string(*(p.name)); // Deep copy
	this->hand = new Hand(*(p.hand)); // use Hand's copy constructor
	this->ordersList = new OrdersList(*(p.ordersList)); // use OrdersList's copy constructor
	if (p.territories != nullptr) {
		this->territories = new vector<Territory*>(*(p.territories)); // Deep copy of vector
	}
	else {
		this->territories = nullptr;
		territories->push_back(new Territory("DefaultTerritory1", nullptr, 0)); // for testing
		territories->push_back(new Territory("DefaultTerritory2", nullptr, 0)); // for testing
		territories->push_back(new Territory("DefaultTerritory3", nullptr, 0)); // for testing
		territories->push_back(new Territory("DefaultTerritory4", nullptr, 0)); // for testing
	}
	cout << "Player " << *this->name << " copied." << std::endl;
}

// Destructor: This destructor releases the memory allocated for the name, hand, ordersList, and territories.
// It also iterates through the territories vector to delete each Territory pointer to prevent memory leaks.
// Finally the territories vector pointer itself is deallocated.
Player::~Player() {
	cout << "Player " << *this->name << " destroyed." << std::endl;
	delete this->name;
	delete this->hand;
	delete this->ordersList;
	for (Territory* t : *this->territories) {
		delete t; // Free each Territory pointer
	}
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

void Player::setName(string* name) {
	delete this->name;
	this->name = new string(*name);
}

string Player::getName() const {
	return *(this->name);
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

// toAttack() method: this method returns a list (vector) of Territory objects which represents the
// territories that the player will attack. In Assignment 1, we only create arbitrary terrtories for testing purposes.
// In assignment 2, we will use the actual territories loaded and assigned to the player at the start of the game.
vector<Territory*> Player::toAttack() {
	// Implementation
	cout << "Player::toAttack() called" << endl;
	// Arbitrary assignment of territories for testing
	vector<Territory*> attackableTerritories;
	Territory* t1 = new Territory("US1", nullptr, 5);
	Territory* t2 = new Territory("US2", nullptr, 4);
	Territory* t3 = new Territory("US3", nullptr, 10);
	Territory* t4 = new Territory("US4", nullptr, 11);
	Territory* t5 = new Territory("US5", nullptr, 20);
	// Set edges (adjacent territories)
	t1->addEdge(t2);
	t2->addEdge(t3);
	t3->addEdge(t4);
	t4->addEdge(t5);
	t5->addEdge(t1);
	// Add to attackable list
	attackableTerritories.push_back(t1);
	attackableTerritories.push_back(t2);
	attackableTerritories.push_back(t3);
	attackableTerritories.push_back(t4);
	attackableTerritories.push_back(t5);

	// memory will be deallocated in PlayerDriver.cpp after testing

	return attackableTerritories;
}

// toDefend() method: the idea is exactly the same with toAttack() method above.
vector<Territory*> Player::toDefend() {
	// Implementation
	cout << "Player::toDefend() called" << endl;
	// Arbitrary assignment of territories for testing
	vector<Territory*> defendingTerritories;
	Territory* t1 = new Territory("CA1", nullptr, 50);
	Territory* t2 = new Territory("CA2", nullptr, 40);
	Territory* t3 = new Territory("CA3", nullptr, 10);
	Territory* t4 = new Territory("CA4", nullptr, 11);
	Territory* t5 = new Territory("CA5", nullptr, 20);
	// Set edges (adjacent territories)
	t1->addEdge(t2);
	t2->addEdge(t3);
	t3->addEdge(t4);
	t4->addEdge(t5);
	t5->addEdge(t1);
	// Add to attackable list
	defendingTerritories.push_back(t1);
	defendingTerritories.push_back(t2);
	defendingTerritories.push_back(t3);
	defendingTerritories.push_back(t4);
	defendingTerritories.push_back(t5);

	// memory will be deallocated in PlayerDriver.cpp after testing
	return defendingTerritories;
}

// issueOrder() method: this method takes an ORDER_TYPE enum as a parameter, which indicates
// the type of order that the player wants to issue. Based on the order type, the method creates
// the corresponding Order object (Deploy, Advance, Bomb, Blockade, Airlift, Negotiate) and adds it to the player's ordersList.
// This is where polymorphism is used, as the ordersList is a collection of Order pointers,
void Player::issueOrder(ORDER_TYPE otype) {
	// Implementation
	cout << "Player::issueOrder() called with order type: " << otype << std::endl;
	// From other type, choose which type of order to create
	switch (otype) {
	case DEPLOY: {
		// Create a Deploy order and add to ordersList
		cout << "Creating Deploy order" << endl;
		this->ordersList->addOrder(new Deploy(this, nullptr, 1));
		break;
	}
	case ADVANCE: {
		// Create an Advance order and add to ordersList
		cout << "Creating Advance order" << endl;
		this->ordersList->addOrder(new Advance(this, 1, nullptr, nullptr));
		break;
	}
	case BOMB: {
		// Create a Bomb order and add to ordersList
		cout << "Creating Bomb order" << endl;
		this->ordersList->addOrder(new Bomb(this, nullptr));
		break;
	}
	case BLOCKADE: {
		// Create a Bomb order and add to ordersList
		cout << "Creating Blockade order" << endl;
		this->ordersList->addOrder(new Blockade(this, nullptr));
		break;
	}
	case AIRLIFT: {
		// Create a Bomb order and add to ordersList
		cout << "Creating Airlift order" << endl;
		this->ordersList->addOrder(new Airlift(this, 1, nullptr, nullptr));
		break;
	}
	case NEGOTIATE: {
		// Create a Bomb order and add to ordersList
		cout << "Creating Negotiate order" << endl;
		this->ordersList->addOrder(new Negotiate(this, nullptr));
		break;
	}
	}
	cout << "Adding order to orders list" << endl;

	// Memory management note: orders should be deallocated after being executed.
}


