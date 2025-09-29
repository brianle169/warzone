#include "Map.h"



Territory::Territory(const std::string& name, Continent* continent, int armies)
	: name(name), continent(continent), armies(armies) {
}
Territory::Territory(Territory& other) {

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
const std::vector<Territory*>& Territory::getEdges() const {
	return this->edges;
}



Continent::Continent(Continent& other) {

}
Continent::Continent(const std::string& name, int bonus) : name(name), bonus(bonus) {}
void Continent::addTerritory(Territory* territory) {
	territories.push_back(territory);
}

std::string Continent::getName() {
	return name;
}
const std::vector<Territory*>& Continent::getTerritories() const {
	return this->territories;
}




Map::Map() {}
Map::Map(Map& other) {}
Map::~Map() {}
const std::unordered_map<std::string, std::unique_ptr<Territory>>& Map::getTerritories() const {
	return territories;
}
const std::unordered_map<std::string, std::unique_ptr<Continent>>& Map::getContinents() const {
	return continents;
}


void Map::addTerritory(std::string& name, Continent* continent, int armies) {
	this->territories.insert({ name, std::make_unique <Territory>(name, continent, armies) });

}
void Map::addContinent(std::string const& name, int bonus) {
	this->continents.insert({ name, std::make_unique<Continent>(name, bonus) });
}
Continent* Map::getContinent(std::string& _name) {
	return this->continents.at(_name).get();

}
Territory* Map::getTerritory(std::string& _name) {
	return this->territories.at(_name).get();
}




bool Map::isConnected() {//BFS search 
	std::queue<Territory*> q;
	q.push(this->getTerritories().begin()->second.get());

	std::set<Territory*> visited;
	visited.insert(this->getTerritories().begin()->second.get());

	while (!q.empty()) {
		for (Territory* edge : q.front()->getEdges()) {
			if (visited.insert(edge).second) {
				q.push(edge);
			}
		}
		q.pop();
	}
	if (visited.size() != this->getTerritories().size()) {
		return false;
	}
	return true;
}




bool Map::continentsConnected() {//BFS search in each continent's terretories 
	for (auto& c : this->getContinents()) {
		std::queue<Territory*> q;
		q.push(c.second->getTerritories()[0]);

		std::set<Territory*> visited;
		visited.insert(c.second->getTerritories()[0]);
		while (!q.empty()) {

			for (Territory* edge : q.front()->getEdges()) {
				if (visited.insert(edge).second) {
					q.push(edge);
				}
			}
			q.pop();
		}
		if (visited.size() != c.second->getTerritories().size()) {
			return false;
		}
	}
	return true;
}
bool Map::territoryExclusive() {
	std::set<Territory*> visited;
	size_t size = 0;
	for (auto& c : this->getContinents()) {
		size += c.second->getTerritories().size();
		for (auto t : (c.second->getTerritories())) {
			if (!visited.insert(t).second) {
				return false;
			}
		}
	}
	if (visited.size() != size) {
		return false;
	}
	return true;
}





MapLoader::MapLoader() {}

std::unique_ptr<Map> MapLoader::load(std::string& filename) {
	std::ifstream file(filename);
	if ((!file.is_open())) {
		return nullptr;
	}
	auto map = std::make_unique<Map>();
	std::string s;
	char currentSection;
	std::vector<std::vector<std::string>> territoryLines;

	while (getline(file, s)) {
		if (s.empty()) {
			continue;
		}
		if (s == "[Continents]") {
			currentSection = 'c';
			continue;
		}
		if (s == "[Territories]") {
			currentSection = 't';
			continue;
		}

		if (currentSection == 'c') {
			auto position = s.find('=');
			if (position == std::string::npos) {
				return nullptr;
			};
			map->addContinent(s.substr(0, position), std::stoi(s.substr(position + 1)));
		}
		if (currentSection == 't') {
			std::istringstream sStream(s);
			std::string word;
			int counter = 0;
			std::vector<std::string> line;
			while (std::getline(sStream, word, ',')) {
				if (counter != 1 && counter != 2) {
					line.push_back(word);
				}
				counter++;
			}territoryLines.push_back(line);
		}
	}
	for (std::vector<std::string> line : territoryLines) { //create territories
		map->addTerritory(line[0], map->getContinent(line[1]), rand() % 7 + 2);
	}

	for (std::vector<std::string> line : territoryLines) {//create territory edges 
		for (int i = 1; i < line.size(); i++) {
			map->getTerritory(line[0])->addEdge(map->getTerritory(line[i]));
		}
	}
	return map;
}
