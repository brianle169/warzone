#include "Map.h"

//Territory
Territory::Territory(const std::string& name, Continent* continent, int armies)
	: name(name), continent(continent), armies(armies) {
	if (this->continent) {//add territory to continent 
		this->continent->addTerritory(this);
	}
}
//Copy constructor 
Territory::Territory(const Territory& other)
	: name(other.name), continent(other.continent), armies(other.armies), edges(other.edges) {
	//part of the same continent in memory and has the same edges
}
// Add an edge to the adjacency list
void Territory::addEdge(Territory* edge) {
	edges.push_back(edge);
}
//getters:
std::string Territory::getName() {
	return this->name;
}
int Territory::getArmies() {
	return armies;
}
const std::vector<Territory*>& Territory::getEdges() const {
	return this->edges;
}
Continent* Territory::getContinent() {
	return this->continent;
}
Player* Territory::getPlayer() {
	return this->player;
}

//setters
void Territory::setPlayer(Player* player) {
	this->player = player;
}

void Territory::setArmies(int armies) {
	this->armies = armies;
}

//assignment
Territory& Territory::operator=(const Territory& other) {
	//part of the same continent in memory and has the same edges
	name = other.name;
	continent = other.continent;
	armies = other.armies;
	edges = other.edges;
	return *this;
}
//stream insertion
std::ostream& operator<<(std::ostream& os, const Territory& t) {
	os << "Territory: " << t.name
		<< " (Armies: " << t.armies << ") , Continent: "
		<< t.continent->getName() << " Edges: ";
	for (auto e : t.edges) {
		os << (e->getName()) << " ";
	}
	return os;
}

// Checks adjacency 
bool Territory::isEdge(Territory* territory) {
	for(Territory* edge: this->edges) {
		if(edge == territory) {
			return true;
		}
	}
	return false;
}

//Continent
//Copy constructor
Continent::Continent(const Continent& other)
	: name(other.name), bonus(other.bonus), territories(other.territories) {
	//Continent owns the same territories in memory, map has ownership
}
Continent::Continent(const std::string& name, int bonus) : name(name), bonus(bonus) {}
void Continent::addTerritory(Territory* territory) {
	territories.push_back(territory);
}
//getters:
std::string Continent::getName() {
	return name;
}
int Continent::getBonus() {
	return this->bonus;
}
const std::vector<Territory*>& Continent::getTerritories() const {
	return this->territories;
}
//assignment 
Continent& Continent::operator=(const Continent& other) {
	name = other.name;
	bonus = other.bonus;
	territories = other.territories;
	return *this;
	//Continent owns the same territories in memory, map has ownership
}
//stream insertion 
std::ostream& operator<<(std::ostream& os, const Continent& c) {
	os << "Continent: " << c.name << " (Bonus: " << c.bonus << ") Territories: ";
	for (auto t : c.territories) {
		os << (t->getName()) << " ";
	}
	return os;
}
//Map
Map::Map(){}
//copy constructor
Map::Map(Map& other) {//deep copy, since map has ownership of unique pointers 
	//copy continents 
	for (const auto& [name, contPtr] : other.continents) {
		continents[name] = std::make_unique<Continent>(*contPtr);
	}
	//copy territories
	for (const auto& [name, terrPtr] : other.territories) {
		Continent* cont = terrPtr->getContinent();
		territories[name] = std::make_unique<Territory>(terrPtr->getName(), cont, terrPtr->getArmies());
	}
	//edges
	for (const auto& [name, terrPtr] : other.territories) {
		Territory* newTerr = territories[name].get();
		for (Territory* edge : terrPtr->getEdges()) {
			newTerr->addEdge(territories[edge->getName()].get());
		}
	}
}
//getters
const std::unordered_map<std::string, std::unique_ptr<Territory>>& Map::getTerritories() const {
	return territories;
}
const std::unordered_map<std::string, std::unique_ptr<Continent>>& Map::getContinents() const {
	return continents;
}
Continent* Map::getContinent(const std::string& name) {
	auto it = this->getContinents().find(name);
	if (it == this->getContinents().end()) {
		return nullptr;
	}
	return it->second.get();

}
Territory* Map::getTerritory(const std::string& name) {

	auto it = this->getTerritories().find(name);
	if (it == this->getTerritories().end()) {
		return nullptr;
	}
	return it->second.get();

}
//adding unique pointers to map 
void Map::addTerritory(std::string& name, Continent* continent, int armies) {
	this->territories.insert({ name, std::make_unique <Territory>(name, continent, armies) });
}
void Map::addContinent(std::string const& name, int bonus) {
	this->continents.insert({ name, std::make_unique<Continent>(name, bonus) });
}
//stream insertion
std::ostream& operator<<(std::ostream& os, const Map& m) {
	os << "Continents:\n";
	for (const auto& [name, cont] : m.continents) {
		os << "  " << *cont << "\n";
	}
	os << "Territories:\n";
	for (const auto& [name, terr] : m.territories) {
		os << "  " << *terr << "\n";
	}
	return os;
}
//assignment
Map& Map::operator=(const Map& other) {
	if (this == &other) return *this;

	continents.clear();
	territories.clear();

	for (const auto& [name, contPtr] : other.continents) {
		continents[name] = std::make_unique<Continent>(*contPtr);
	}
	for (const auto& [name, terrPtr] : other.territories) {
		Continent* cont = terrPtr->getContinent();
		territories[name] = std::make_unique<Territory>(terrPtr->getName(), cont, terrPtr->getArmies());
	}
	for (const auto& [name, terrPtr] : other.territories) {
		Territory* newTerr = territories[name].get();
		for (Territory* edge : terrPtr->getEdges()) {
			newTerr->addEdge(territories[edge->getName()].get());
		}
	}
	return *this;
}

//map verification:
bool Map::isConnected() {//BFS search 
	if (this->getTerritories().empty()) {
		return false;
	}
	std::queue<Territory*> q;
	q.push(this->getTerritories().begin()->second.get());//add first t to q

	std::set<Territory*> visited;
	visited.insert(this->getTerritories().begin()->second.get());//add first t to visited
	while (!q.empty()) {
		for (Territory* edge : q.front()->getEdges()) {
			if (visited.insert(edge).second) {//sets will not allow duplicates
				q.push(edge);//if just now visited, add to q
			}
		}
		q.pop();//go to the next in the q
	}
	if (visited.size() != this->getTerritories().size()) {
		return false;
	}
	return true;
}

bool Map::continentsConnected() {//BFS search in strictly each continent's terretories 
	for (auto& c : this->getContinents()) {
		const auto& terrs = c.second->getTerritories();
		if (terrs.empty() || !terrs[0]) {
			return false;
		}

		std::queue<Territory*> q;
		q.push(terrs[0]);

		std::set<Territory*> visited;
		visited.insert(terrs[0]);

		while (!q.empty()) {

			for (Territory* edge : q.front()->getEdges()) {
				if (edge && edge->getContinent() && edge->getContinent() == c.second.get() 
					&& visited.insert(edge).second) {
					//checks if edge is in the continent, and if they each exist
					q.push(edge);
				}
			}
			q.pop();
		}
		if (visited.size() != c.second->getTerritories().size()) {
			return false;
		}
	}

	

bool Map::territoryExclusive() {//adds every t in every c to a set
	std::set<Territory*> visited;
	size_t size = 0;
	for (auto& c : this->getContinents()) {
		size += c.second->getTerritories().size();
		for (auto t : (c.second->getTerritories())) {
			if (!visited.insert(t).second) {//if t is a duplicate (part of >1 c)
				return false;
			}
		}
	}
	if (visited.size() != size) {
		return false;
	}
	return true;
}
//calls all three validation functions
bool Map::validate() {
	if (!this->isConnected()) {
		std::cout << "Map is not connected\n";
		return false;
	}
	std::cout << "Map is connected\n";

	if (!this->continentsConnected()) {
		std::cout << "Map's continents not are not connected\n";
		return false;
	}
	std::cout << "Map's continents are connected\n";


	if (!this->territoryExclusive()) {
		std::cout << "Map's territories are not exclusive\n";
		return false;
	}
	std::cout << "Map's territories are exclusive\n";

	return true;
}

//Map loader (only one function)
MapLoader::MapLoader(){}
std::unique_ptr<Map> MapLoader::load(const std::string& filename) {
	std::ifstream file(filename);
	if ((!file.is_open())) {//open file and verify
		std::cout << "\nCould not open: " << filename << "\n";
		return nullptr;
	}
	auto map = std::make_unique<Map>();//create empty map

	std::string s;
	char currentSection = 'a';
	std::vector<std::vector<std::string>> territoryLines;

	//will return nullptr if the map file is invalid 
	while (getline(file, s)) {

		if (s.empty()) {
			continue;
		}
		if (s == "[Continents]") {
			currentSection = 'c';
			continue;
		}
		if (s == "[Territories]") {
			if (currentSection != 'c') {
				return nullptr;
			}
			currentSection = 't';
			continue;
		}

		if (currentSection == 'c') {//parse continents 
			auto position = s.find('=');
			if (position == std::string::npos) {//if there is no '='
				return nullptr;
			};
			//add continent using name and bonus number
			map->addContinent(s.substr(0, position), std::stoi(s.substr(position + 1)));
		}
		if (currentSection == 't') {//parse territories
			std::istringstream sStream(s);//convert line into a stream
			std::string word;
			int counter = 0;
			std::vector<std::string> line; //turn line in to a vector of strings
			
			while (std::getline(sStream, word, ',')) {
				if (counter != 1 && counter != 2) {//skip coordinates
					line.push_back(word);
				}
				counter++;
			}
			if (line.size() < 2) {
				return nullptr;
			}
			territoryLines.push_back(line);
		}
		continue;
	}
	for (std::vector<std::string> line : territoryLines) { //create territories from parsed lines
		//army amount is 0
		if (!map->getContinent(line[1])) {
			return nullptr;
		}
		map->addTerritory(line[0], map->getContinent(line[1]), 0);
	}
	for (std::vector<std::string> line : territoryLines) {//create territory edges 
		for (int i = 2; i < line.size(); i++) {
			if (!map->getTerritory(line[0]) || !map->getTerritory(line[i])) {
				return nullptr;
			}
			map->getTerritory(line[0])->addEdge(map->getTerritory(line[i]));
		}
	}
	file.close();
	return map;
}