#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <set>
#include <filesystem>

class Continent;
class Player;
class Territory {
public:

	Territory(const std::string& name, Continent* continent, int armies);
	Territory(Territory& other);

	void addEdge(Territory* edge);

	std::string getName();
	const std::vector<Territory*>& getEdges() const;

	int getArmies();
	Continent* getContinent();


private:
	std::vector<Territory*> edges;
	std::string name;
	Continent* continent;
	int armies;
	Player* player;
};
class Continent {
public:

	Continent(Continent& other);

	Continent(const std::string& name, int bonus);
	void addTerritory(Territory*);

	std::string getName();
	const std::vector<Territory*>& getTerritories() const;
private:
	std::vector<Territory*> territories;
	std::string name;
	int bonus;
};

class Map {
public:
	Map();
	Map(Map& other);


	void addTerritory(std::string& name, Continent* continent, int armies);
	void addContinent(std::string const& name, int bonus);
	Continent* getContinent(std::string& name);
	Territory* getTerritory(std::string& name);
	const std::unordered_map<std::string, std::unique_ptr<Territory>>& getTerritories() const;
	const std::unordered_map<std::string, std::unique_ptr<Continent>>& getContinents() const;

	bool isConnected();
	bool continentsConnected();
	bool territoryExclusive();


private:
	std::unordered_map<std::string, std::unique_ptr<Territory>> territories;
	std::unordered_map<std::string, std::unique_ptr<Continent>> continents;


};
class MapLoader {
public:
	MapLoader();

	std::unique_ptr<Map> load(const std::string&);

private:

};
