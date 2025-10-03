#include <iostream>
#include "Map.h"

void testLoadMaps() {
	std::vector<std::string> fileNames = {
	R"(C:\Users\owen\Downloads\_62_small - CASTLE MOONBAT\_62_ small - CASTLE MOONBAT.map)",
	R"(C:\Users\owen\Downloads\Flower K\Flower K.map)",
	R"(C:\Users\owen\Downloads\Australia\Australia.map)",
	R"(C:\Users\owen\Downloads\emptytext.txt)"
	};
	std::vector<std::unique_ptr<Map>> maps;

	MapLoader loader;
	int i = 0;
	for (const std::string file : fileNames) {

		auto map = loader.load(file);

		if (!map || (map == nullptr)) {
			std::cout << "Failed to load map: " << file << "\n";
			continue;
		}


		if (!map->isConnected()) {
			std::cout << "Map " << (i + 1) << " is not connected\n";
			continue;
		}
		std::cout << file << "connected\n";

		if (!map->continentsConnected()) {
			std::cout << "Map " << (i + 1) << "'s continents not connected\n";
			continue;
		}
		std::cout << file << "CONTconnected\n";

		if (!map->territoryExclusive()) {
			std::cout << "Map " << (i + 1) << "'s territories are not exclusive\n";
			continue;
		}
		std::cout << file << "exclusive\n";
		i++;
		maps.push_back(std::move(map));
	}
}
int main() {
	testLoadMaps();
	std::cout << "good";
	std::cin.get();
	return 0;
}