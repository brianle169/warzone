#include <iostream>
#include "Map.h"

void testLoadMaps() {
	std::vector<std::string> fileNames;
	std::vector<std::unique_ptr<Map>> maps;

	MapLoader loader;
	int i = 0;
	for (auto file : fileNames) {
		auto map = loader.load(file);

		if (!map->isConnected()) {
			std::cout << "Map " << (i + 1) << " is not connected\n";
		}

		if (!map->continentsConnected()) {
			std::cout << "Map " << (i + 1) << "'s continents not connected\n";
		}

		if (!map->territoryExclusive()) {
			std::cout << "Map " << (i + 1) << "'s territories are not exclusive\n";
		}
		i++;
		maps.push_back(std::move(map));

	}
}