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
class Player;

class Continent;
class Territory {
public:
	//constructors
	Territory(const std::string& name, Continent* continent, int armies);
	Territory(const Territory& other);

	void addEdge(Territory* edge);

	//getters
	std::string getName();
	const std::vector<Territory*>& getEdges() const;
	int getArmies();
	Continent* getContinent();
	Player* getPlayer();

	//setters
	void setPlayer(Player* player);
	void setArmies(int armies);

	bool isEdge(Territory* territory);

	Territory& operator=(const Territory& other);
	friend std::ostream& operator<<(std::ostream& os, const Territory& t);

private:
	std::vector<Territory*> edges;
	std::string name;
	Continent* continent;
	int armies;
	Player* player;
};
class Continent {
public:

	
	//constructors
	Continent(const std::string& name, int bonus);
	Continent(const Continent& other);

	void addTerritory(Territory*);

	//getters
	std::string getName();
	const std::vector<Territory*>& getTerritories() const;
	int getBonus();

	Continent& operator=(const Continent& other);
	friend std::ostream& operator<<(std::ostream& os, const Continent& c);
private:
	std::vector<Territory*> territories;
	std::string name;
	int bonus;
};

class Map {
public:
	//constructors
	Map();
	Map(Map& other);

	
	void addTerritory(std::string& name, Continent* continent, int armies);
	void addContinent(std::string const& name, int bonus);

	//getters
	Continent* getContinent(const std::string& name);
	Territory* getTerritory(const std::string& name);
	const std::unordered_map<std::string, std::unique_ptr<Territory>>& getTerritories() const;
	const std::unordered_map<std::string, std::unique_ptr<Continent>>& getContinents() const;


	friend std::ostream& operator<<(std::ostream& os, const Map& m);
	Map& operator=(const Map& other);
	
	//verification
	bool isConnected();
	bool continentsConnected();
	bool territoryExclusive();
	bool validate();

	
private:
	std::unordered_map<std::string, std::unique_ptr<Territory>> territories;
	std::unordered_map<std::string, std::unique_ptr<Continent>> continents;
};

class MapLoader {
public:
	MapLoader();
	std::unique_ptr<Map> load(const std::string&);
};
//free func
void testLoadMaps();
// int main();
