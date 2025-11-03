#include <iostream>
#include "Map.h"

void testLoadMaps() {
	std::vector<std::string> fileNames = {// MAP FILE NAMES TO LOAD
	R"(_62_small - CASTLE MOONBAT\_62_ small - CASTLE MOONBAT.map)",
	R"(Flower K.map)",
	R"(Australia.map)",
	R"(InvalidMap.txt)"
	};
	std::vector<std::unique_ptr<Map>> maps;

	MapLoader loader;
	
	for (const std::string file : fileNames) {

		auto map = loader.load(file);

		if (!map) {
			std::cout << "Failed to load map: " << file << "\n";
			continue;
		}


		if (!map->validate()) {
			continue;
		}

		std::cout << "Map " << file << " VALIDATED\n\n";
		std::cout << *map << std::endl;
		maps.push_back(std::move(map));
	}
}
int main() {
	testLoadMaps();
	return 0;
}