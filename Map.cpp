#include "Map.h";

Territory::Territory() {
}
Territory::Territory(Territory& other) {

}

Territory::~Territory(){}
void Territory::addEdge(Territory* edge) {
	edges.push_back(edge);

}

std::string Territory::getName(){
	return this->name;
}
std::string Territory::getContinentName(){
	return continentName;
}
int Territory::getArmies() {
	return armies;
}






Continent::Continent() {

}
void Continent::addTerritory(Territory* territory) {
	territories.push_back(territory);
}
