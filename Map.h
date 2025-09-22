#pragma once
#include <iostream>
#include <vector>
#include <string>
class Territory {
public:	
	Territory();
	Territory(Territory& other);
	~Territory();
	void addEdge(Territory*);

	std::string getName();
	std::string getContinentName();
	int getArmies();

private:
	std::vector<Territory*> edges;
	std::string name;
	std::string continentName;
	int armies;
};
class Continent {
public:
	Continent();
	Continent(Continent& other);
	~Continent();
	void addTerritory(Territory*);

	std::string getName();
private:
	std::vector<Territory*> territories;
	std::string name;
};

class Map{
public:
	Map();
	Map(Map& other);
	~Map();
private:

};

