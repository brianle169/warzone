#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <limits>

#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "Map.h"
#include "GameEngine.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////

// Player class implementation
// Default constructor: This constructor instantiate an object with default values:
// -  name is an empty string
// -  hand is a new empty Hand object
// -  ordersList is a new empty OrdersList object
// -  territories is an empty vector of Territory pointers, which will be assigned later by the game engine
Player::Player() : name(new string("")),
				   hand(new Hand()),
				   ordersList(new OrdersList()),
				   territories(new vector<Territory *>()),
				   attacking(new vector<Territory *>()),
				   defending(new vector<Territory *>()),
				   reinforcementPool(new int(0)),
				   issueOrderStatus(new array<bool, 4>())
{
}

// Parameterized constructor: This constructor takes a string pointer as a parameter to set the player's name.
// The other attributes are initialized similarly to the default constructor.
Player::Player(string name) : hand(new Hand()),
							  ordersList(new OrdersList()),
							  territories(new vector<Territory *>()),
							  attacking(new vector<Territory *>()),
							  defending(new vector<Territory *>()),
							  reinforcementPool(new int(0)),
							  issueOrderStatus(new array<bool, 4>())
{
	this->name = new string(name); // This is a pointer assignment.
}

// Copy constructor: This constructor creates a new Player object as a deep copy of an existing Player object.
// It allocates new memory for the name, hand, ordersList, and territories to ensure that the new object has its own copies of these attributes.
// If the source player's territories is null, it initializes territories with default values for testing.
Player::Player(const Player &p)
{
	this->name = new string(*(p.name));						   // Deep copy
	this->hand = new Hand(*(p.hand));						   // use Hand's copy constructor
	this->ordersList = new OrdersList(*(p.ordersList));		   // use OrdersList's copy constructor
	this->reinforcementPool = new int(*(p.reinforcementPool)); // Deep copy
	if (p.territories != nullptr)
		this->territories = new vector<Territory *>(*(p.territories)); // Deep copy of vector
	else
		this->territories = new vector<Territory *>();

	if (p.attacking != nullptr)
		this->attacking = new vector<Territory *>(*(p.attacking)); // Deep copy of vector
	else
		this->attacking = new vector<Territory *>();

	if (p.defending != nullptr)
		this->defending = new vector<Territory *>(*(p.defending)); // Deep copy of vector
	else
		this->defending = new vector<Territory *>();
	this->issueOrderStatus = new array<bool, 4>(*(p.issueOrderStatus)); // Deep copy of array
}

// Destructor: This destructor releases the memory allocated for the name, hand, ordersList, and territories.
// It also iterates through the territories vector to delete each Territory pointer to prevent memory leaks.
// Finally the territories vector pointer itself is deallocated.
Player::~Player()
{
	cout << "Player " << *this->name << " destroyed." << std::endl;
	delete this->name;
	delete this->hand;
	delete this->ordersList;
	delete this->reinforcementPool;
	delete this->territories;
	delete this->attacking;
	delete this->defending;
	delete this->issueOrderStatus;
}

// Below are the getters and setters for each attribute of the Player class.
// Setters ensure that the existing memory is deallocated before assigning new values.
void Player::setTerritories(vector<Territory *> *terrs)
{
	for (Territory *t : *this->territories)
		delete t; // Free each Territory pointer
	this->territories = terrs;
}

void Player::addTerritory(Territory *terr)
{
	this->getTerritories()->push_back(terr);
}

vector<Territory *> *Player::getTerritories() const
{
	return this->territories;
}

void Player::setHand(Hand *h)
{
	if (this->hand != h)
	{
		delete this->hand;
		this->hand = h;
	}
}

Hand *Player::getHand() const
{
	return this->hand;
}

void Player::setOrdersList(OrdersList *ol)
{
	if (this->ordersList != ol)
	{
		delete this->ordersList;
		this->ordersList = ol;
	}
}

OrdersList *Player::getOrdersList() const
{
	return this->ordersList;
}

void Player::setName(string name)
{
	delete this->name;
	this->name = new string(name);
}

string Player::getName() const
{
	return *(this->name);
}

void Player::setReinforcementPool(int num)
{
	*(this->reinforcementPool) = num;
}

int Player::getReinforcementPool() const
{
	return *this->reinforcementPool;
}

// Stream insertion operator overloading: This operator prints out general information about the Player object.
// Currently for testing, the Player's Hand object is empty, and OrdersList is also initially empty, so to check
// if they exist, we only check whether the pointers are pointing to nullptr or not.
// However, the territories vector is printed out in detail, showing each territory's name and number of armies.
ostream &operator<<(ostream &out, const Player &player)
{
	out << "Player Name: " << *(player.name) << endl;
	out << "Reinforcement Pool: " << *(player.reinforcementPool) << endl;
	out << "Territories Owned: " << player.territories->size() << endl;
	out << *player.hand << endl;
	return out; // out = output stream
}

// Assignment operator overloading: This operator assigns the values from one Player object to another.
// It first checks for self-assignment, then deallocates existing memory before performing deep copies of the attributes.
Player &Player::operator=(const Player &p)
{
	// If not comparing to itself
	if (this != &p)
	{
		// Clear the members of this object
		delete this->name;
		delete this->hand;
		delete this->ordersList;
		for (Territory *t : *this->territories)
			delete t;										// Free each Territory pointer
		this->name = new string(*(p.name));					// Deep copy
		this->hand = new Hand(*(p.hand));					// use Hand's copy constructor
		this->ordersList = new OrdersList(*(p.ordersList)); // use OrdersList's copy constructor
		this->territories = p.getTerritories();
	}

	return *this;
}

// getAttackableTerritories() method: return a list of territories that the player can attack.
// these territories are adjacent to the player's own territories and are not owned by the player.
unordered_map<string, Territory *> Player::getAttackableTerritories()
{
	unordered_map<string, Territory *> res;

	if (!this->territories || this->territories->empty())
		return res;

	for (Territory *owned : *this->territories)
	{
		if (!owned)
			continue;

		const vector<Territory *> &adj = owned->getEdges(); // assume returns const ref or copy
		for (Territory *neighbor : adj)
		{
			if (!neighbor)
				continue;

			Player *owner = neighbor->getPlayer();
			// Attackable if unowned OR owned by someone else
			if (owner != this)
			{
				// Insert only once
				res.emplace(neighbor->getName(), neighbor);
			}
		}
	}
	return res;
}

unordered_map<string, Territory *> Player::getDefendableTerritories()
{
	unordered_map<string, Territory *> res;
	for (Territory *terr : *this->territories)
	{
		if (terr)
		{
			res.emplace(terr->getName(), terr);
		}
	}
	return res;
}

// toAttack() method: this method returns a list (vector) of Territory objects which represents the
// territories that the player will attack. In Assignment 1, we only create arbitrary terrtories for testing purposes.
// In assignment 2, we will use the actual territories loaded and assigned to the player at the start of the game.
vector<Territory *> Player::toAttack()
{
	cout << "Available territories to attack: " << endl;
	unordered_map<string, Territory *> attackableTerritories = this->getAttackableTerritories();
	for (const auto &pair : attackableTerritories)
	{
		cout << "- " << pair.first << " (" << pair.second->getArmies() << ")" << endl;
	}
	cout << endl;

	string input = "";
	do
	{
		cout << "Select territories to attack (Enter 'x' to finish) \n>> ";
		cin >> input;
		if (input == "x" || input == "X")
			break;
		try
		{
			Territory *terr = attackableTerritories.at(input);
			if (std::find(this->attacking->begin(), this->attacking->end(), terr) != this->attacking->end())
			{
				cout << "Territory " << terr->getName() << " has already been selected for attack." << endl;
				continue;
			}
			cout << "Territory " << terr->getName() << " selected for attack." << endl;
			this->attacking->push_back(terr); // insert the territory pointer into attacking vector
			this->displayTerritories(*this->attacking);
		}
		catch (const out_of_range &)
		{
			cout << "Invalid territory. Please try again." << endl;
		}
	} while (true);
	return *this->attacking;
}

// toDefend() method: the idea is exactly the same with toAttack() method above.
vector<Territory *> Player::toDefend()
{
	cout << "Available territories to defend: " << endl;
	unordered_map<string, Territory *> defendableTerritories = this->getDefendableTerritories();
	for (const auto &pair : defendableTerritories)
	{
		cout << "- " << pair.first << " (" << pair.second->getArmies() << ")" << endl;
	}
	cout << endl;

	string input = "";
	do
	{
		cout << "Select territories to defend (Enter 'x' to finish) \n>> ";
		cin >> input;
		if (input == "x" || input == "X")
			break;
		try
		{
			Territory *terr = defendableTerritories.at(input);
			if (std::find(this->defending->begin(), this->defending->end(), terr) != this->defending->end())
			{
				cout << "Territory " << terr->getName() << " has already been selected for defend." << endl;
				continue;
			}
			cout << "Territory " << terr->getName() << " selected for defend." << endl;
			this->defending->push_back(terr); // insert the territory pointer into defending vector
			this->displayTerritories(*this->defending);
		}
		catch (const out_of_range &)
		{
			cout << "Invalid territory. Please try again." << endl;
		}
	} while (true);
	return *this->defending;
}

void Player::displayTerritories(const std::vector<Territory *> &territories)
{
	for (Territory *t : territories)
	{
		if (t)
			std::cout << t->getName() << " - " << t->getArmies() << '\n';
	}
}

// print the orders list
void Player::displayOrdersList(const OrdersList *ordersList)
{
	if (!ordersList)
	{
		cout << "OrdersList size: 0\n";
		return;
	}
	else
	{
		cout << "OrdersList contains: " << endl;
		for (int i = 0; i < static_cast<int>(ordersList->size()); i++)
		{
			if (ordersList->getOrder(i))
			{
				cout << i << ". " << *ordersList->getOrder(i) << endl;
			}
		}
	}
}

Deploy *Player::deploy(vector<Territory *> &defendingTerritories)
{
	cout << "===========================================" << endl;
	cout << "============Deployment Phase===============" << endl;
	cout << "===========================================" << endl;
	cout << "\nDefending List: " << endl;
	int count = 0;
	for (Territory *t : defendingTerritories)
	{
		cout << count << ". " << t->getName() << " (" << t->getArmies() << ") " << endl;
		count++;
	}
	int numArmies = 0, territoryIndex = 0;
	bool validInput = false;
	while (!validInput)
	{
		cout << "Number of armies to deploy \n>> ";
		cin >> numArmies;
		if (cin.fail())
		{
			cin.clear();										 // clear the fail state
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		if (numArmies > 0 && numArmies <= *(this->reinforcementPool))
		{
			validInput = true;
			this->setReinforcementPool(*(this->reinforcementPool) - numArmies);
		}
		else
			cout << "Invalid number of armies. Please enter a number between 1 and " << *(this->reinforcementPool) << "." << endl;
	}
	validInput = false;
	while (!validInput)
	{
		cout << "Select territory to deploy to (please use the index provided) \n>> ";
		cin >> territoryIndex;
		if (cin.fail())
		{
			cin.clear();										 // clear the fail state
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		if (territoryIndex >= 0 && territoryIndex < static_cast<int>(defendingTerritories.size()))
			validInput = true;
		else
			cout << "Invalid territory index. Please try again." << endl;
	}
	// cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return new Deploy(this, defendingTerritories[territoryIndex], numArmies);
}

Advance *Player::advance(vector<Territory *> &attackingTerritories, vector<Territory *> &defendingTerritories)
{
	cout << "===========================================" << endl;
	cout << "============Advancement Phase==============" << endl;
	cout << "===========================================" << endl;

	cout << "\nAdvance orders are optional. Do you want to issue an Advance order?\nYes (y) to issue an Advance order. No (n) to move on to other orders (y/n) \n>> ";
	string choice;
	cin >> choice;
	if (choice != "y" && choice != "Y")
	{
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		this->issueOrderStatus->at(static_cast<int>(IssuePhase::AdvancePhase)) = true;
		return nullptr; // No advance order issued
	}

	// Setup source and target territories
	Territory *sourceTerritory = nullptr, *targetTerritory = nullptr;
	vector<Territory *> possibleTargets;
	possibleTargets.insert(possibleTargets.end(), attackingTerritories.begin(), attackingTerritories.end());
	possibleTargets.insert(possibleTargets.end(), defendingTerritories.begin(), defendingTerritories.end());

	// Choose a source territory from the user's own territories
	cout << "\nCurrent territories you own: " << endl;
	int count = 0;
	for (Territory *t : *this->territories)
	{
		cout << count << ". " << t->getName() << " (" << t->getArmies() << ") " << endl;
		count++;
	}

	int sourceIndex = -1;
	bool validInput = false;
	while (!validInput)
	{
		cout << "\nChoose a source territory to advance from (using the index provided) >> ";
		cin >> sourceIndex;
		if (cin.fail())
		{
			cin.clear();										 // clear the fail state
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		if (sourceIndex >= 0 && sourceIndex < static_cast<int>(this->territories->size()))
			validInput = true;
		else
			cout << "Invalid territory index. Please try again." << endl;
	}
	sourceTerritory = this->territories->at(sourceIndex);

	int numArmies = 0;
	validInput = false;
	while (!validInput)
	{
		cout << "\nNumber of armies to advance >> ";
		cin >> numArmies;
		if (cin.fail())
		{
			cin.clear();										 // clear the fail state
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		else
		{
			validInput = true;
		}
	}

	cout << "\nChoose a target territory to advance to from the two lists below." << endl;
	count = 0;
	validInput = false;

	cout << "Attacking territories: " << endl;
	for (Territory *t : attackingTerritories)
	{
		cout << count << ". " << t->getName() << endl;
		count++;
	}

	cout << "Defending territories: " << endl;
	for (Territory *t : defendingTerritories)
	{
		cout << count << ". " << t->getName() << endl;
		count++;
	}

	int targetIndex = -1;
	while (!validInput)
	{
		cout << "\nSelect target territory to advance to (using the index provided) >> ";
		cin >> targetIndex;
		if (cin.fail())
		{
			cin.clear();										 // clear the fail state
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		if (targetIndex >= 0 && targetIndex < static_cast<int>(possibleTargets.size()))
			validInput = true;
		else
			cout << "Invalid territory index. Please try again." << endl;
	}
	targetTerritory = possibleTargets[targetIndex];

	return new Advance(this, numArmies, sourceTerritory, targetTerritory);
}

void Player::cardOrder()
{
	cout << "===========================================" << endl;
	cout << "==========Issue Card Order Phase===========" << endl;
	cout << "===========================================" << endl;
	do
	{
		cout << "Select a card to play by its index (or type -1 to skip/finish issuing card orders) \n>> ";
		int cardIndex = -2;
		cin >> cardIndex;
		if (cin.fail())
		{
			cin.clear();										 // clear the fail state
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		if (cardIndex == -1)
		{
			this->issueOrderStatus->at(static_cast<int>(IssuePhase::OtherPhase)) = true;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return; // skip issuing other orders
		}
		if (cardIndex < 0 || cardIndex >= this->hand->getNumCards())
		{
			cout << "Invalid card index. Please try again." << endl;
			continue;
		}
		// Play the selected card
		SpCard cardToPlay = this->hand->getCardAt(cardIndex);
		cardToPlay->play(*GameEngine::getCardDeck(), *this);
		this->hand->remove(cardIndex); // Remove the card from hand after playing
		if (this->hand->getNumCards() == 0)
		{
			this->issueOrderStatus->at(static_cast<int>(IssuePhase::OtherPhase)) = true;
		}
		// Add the card back to the deck
		GameEngine::getCardDeck()->add(cardToPlay);
		return;
	} while (true);
}

bool Player::isDoneIssuingOrder()
{
	return this->issueOrderStatus->at(static_cast<int>(IssuePhase::DeployPhase)) &&
		   this->issueOrderStatus->at(static_cast<int>(IssuePhase::AttackDefendPhase)) &&
		   this->issueOrderStatus->at(static_cast<int>(IssuePhase::AdvancePhase)) &&
		   this->issueOrderStatus->at(static_cast<int>(IssuePhase::OtherPhase));
}

// Note that the issueOrder() will be called repeatedly in each round robin turn of the players until all players are done issuing orders.
void Player::issueOrder()
{
	// 1. Print the essential info: name, reinforcement pool, territories, hand, orders list
	cout << "\n\n==========================" << endl;
	cout << "=== Player " << *(this->name) << "'s turn ===" << endl;
	cout << *this;
	this->displayOrdersList(this->ordersList);
	cout << "==========================" << endl;
	cout << endl;

	vector<Territory *> attackingTerritories;
	vector<Territory *> defendingTerritories;
	// 2. Decide territories to attack and defend.
	if (!this->issueOrderStatus->at(static_cast<int>(IssuePhase::AttackDefendPhase)))
	{
		attackingTerritories = this->toAttack();
		defendingTerritories = this->toDefend();
		this->issueOrderStatus->at(static_cast<int>(IssuePhase::AttackDefendPhase)) = true;
		return;
	}
	else
	{
		cout << "Choosing territories to attack and defend has been completed." << endl;
		attackingTerritories = *(this->attacking);
		defendingTerritories = *(this->defending);
	}
	// 3. After choosing the territories to attack and defend, we will issue
	// deploy orders on the defending territories until the reinforcement pool is empty.
	if (*(this->reinforcementPool) > 0)
	{
		// if the reinforcement pool is not empty, we only have one order option: Deploy
		Order *deployOrder = this->deploy(defendingTerritories);
		this->ordersList->addOrder(deployOrder);
		return;
	}
	else
	{
		cout << "Reinforcement pool is empty. No deploy orders can be issued." << endl;
		this->issueOrderStatus->at(static_cast<int>(IssuePhase::DeployPhase)) = true;
	}
	// 4. After deploying all reinforcements, we can issue other types of orders.
	// *Note: the following orders are optional depending on what the player wants to do.
	// That means they can skip issuing these orders if they want to
	if (!this->issueOrderStatus->at(static_cast<int>(IssuePhase::AdvancePhase)))
	{
		// If advance phase orders have not been issued, we can issue them now.
		Order *advanceOrder = this->advance(attackingTerritories, defendingTerritories);
		if (advanceOrder != nullptr)
			this->ordersList->addOrder(advanceOrder);
		return;
	}
	else
	{
		cout << "Advance orders are finalized. Now you can issue other types using corresponding cards." << endl;
	}

	// 5. Finally, we can issue other types of orders based on the cards in hand.
	if (!this->issueOrderStatus->at(static_cast<int>(IssuePhase::OtherPhase)))
	{
		this->cardOrder();
	}
	else
	{
		cout << "All possible orders have been issued for this turn." << endl;
	}
}

bool Player::hasAllTerritories()
{
	// Idea: loop through the player's territories, and check if each territory is in the current game map's territories.
	if (this->territories->size() != GameEngine::getGameMap()->getTerritories().size())
		return false; // early return if the player has fewer territories than the total number of territories in the map
	const auto &gameMapTerritories = GameEngine::getGameMap()->getTerritories();
	for (Territory *t : *this->territories)
	{
		if (gameMapTerritories.find(t->getName()) == gameMapTerritories.end())
			return false; // if any territory is not found, return false
	}
	return true; // all territories are found
}

void Player::clearIssueOrderStatus()
{
	for (size_t i = 0; i < this->issueOrderStatus->size(); i++)
	{
		this->issueOrderStatus->at(i) = false;
	}
}

void Player::addNegotiatedPlayers(Player *p)
{
	negotiatedPlayers.push_back(p);
}

bool Player::isNegotiatedWith(Player *p) const
{
	return std::find(negotiatedPlayers.begin(), negotiatedPlayers.end(), p) != negotiatedPlayers.end();
}

Player *Player::neutralPlayer = new Player("Neutral");

Player *Player::getNeutralPlayer()
{
	return neutralPlayer;
}
