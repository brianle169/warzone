#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "Orders.h"

// void testGameStates()
// {
//     GameEngine engine;
//     cout << "Welcome to Warzone Game Engine version A1!" << endl;
//     cout << engine << endl;
//     string line;
//     while (engine.getCurrentStateName() != "end")
//     {
//         cout << "> ";
//         // EOF or input error check
//         if (!getline(cin, line))
//         {
//             break;
//         }
//         // Ignore empty user command inputs
//         if (line.empty())
//         {
//             continue;
//         }
//         // Route the user command to the FSM
//         engine.executeCommand(line);
//         // Show the resulting state after the user command
//         cout << engine << endl;
//     }
// }

Card *createCard(const int typeID)
{
    switch (typeID)
    {
    case 0:
        return new BombCard();
    case 1:
        return new AirliftCard();
    case 2:
        return new BlockadeCard();
    case 3:
        return new DiplomacyCard();
    default:
        return nullptr;
    }
}

void testMainGameLoop()
{
    // // Set up the environment
    // GameEngine engine;

    // // 1. Load and validate a map
    // std::string file = R"(_62_ small - CASTLE MOONBAT.map)";
    // std::unique_ptr<Map> map;

    // MapLoader loader;

    // map = loader.load(file);

    // if (!map)
    // {
    //     std::cout << "Failed to load map: " << file << "\n";
    // }

    // if (!map->validate(file))
    // {
    //     std::cout << "Map " << file << " is INVALID\n\n";
    //     return;
    // }

    // std::cout << "Map " << file << " VALIDATED\n\n";
    // std::cout << *map << std::endl;

    // GameEngine::setGameMap(map.release());

    // // 2. Create a deck of cards
    // Deck *deck = new Deck();
    // // Create random 50 cards and add to deck
    // for (int i = 0; i < 50; i++)
    // {
    //     Card *card = createCard(i % 4);
    //     if (card)
    //     {
    //         deck->add(std::shared_ptr<Card>(card));
    //     }
    // }

    // GameEngine::setCardDeck(deck);

    // // 3. Add players, each of them will be given 5 territories, 10 armies to begin with, and draw 2 cards from the deck
    // std::vector<Player *> players;
    // Player *player1 = new Player("Minh");
    // player1->setReinforcementPool(10);
    // player1->getHand()->add(GameEngine::getCardDeck()->draw());
    // player1->getHand()->add(GameEngine::getCardDeck()->draw());

    // Player *player2 = new Player("Owen");
    // player2->setReinforcementPool(10);
    // player2->getHand()->add(GameEngine::getCardDeck()->draw());
    // player2->getHand()->add(GameEngine::getCardDeck()->draw());

    // players.push_back(player1);
    // players.push_back(player2);
    // GameEngine::setPlayers(players);

    // // 4. Assign territories to players. Each player has 5 territories to begin. The territories have to be adjacent
    // for (size_t i = 0; i < players.size(); i++)
    // {
    //     std::vector<Territory *> *playerTerritories = new std::vector<Territory *>();
    //     int territoryCount = 0;
    //     // Pick randomly one territory from the map to start. Then assign adjacent territories until each player has 5 territories.
    //     Territory *territory = nullptr;
    //     int counter = 0;
    //     int offset = 8;
    //     for (const auto &pair : GameEngine::getGameMap()->getTerritories())
    //     {
    //         if (counter == offset * (static_cast<int>(i) + 1))
    //         {
    //             territory = pair.second.get();
    //             break;
    //         }
    //         counter++;
    //     }
    //     if (territory->getPlayer() != nullptr)
    //     {
    //         continue;
    //     }
    //     // Assign the starting territory to the player
    //     playerTerritories->push_back(territory);
    //     territory->setPlayer(players[i]);
    //     territoryCount++;

    //     // Assign adjacent territories until the player has 5
    //     while (territoryCount < 5)
    //     {
    //         std::vector<Territory *> adjacentTerritories = territory->getEdges();
    //         if (adjacentTerritories.empty())
    //         {
    //             break;
    //         }
    //         // Randomly select one of the adjacent territories
    //         territory = adjacentTerritories[rand() % adjacentTerritories.size()];
    //         if (territory->getPlayer() != nullptr || std::find(playerTerritories->begin(), playerTerritories->end(), territory) != playerTerritories->end())
    //         {
    //             continue; // already assigned to this player
    //         }
    //         territory->setPlayer(players[i]);
    //         playerTerritories->push_back(territory);
    //         territoryCount++;
    //     }
    //     players[i]->setTerritories(playerTerritories);
    // }

    // // 5. Start the main game loop

    // // print player's territories
    // // vector<Territory *> *p_terrs = player1->getTerritories();
    // // for (int i = 0; i < static_cast<int>(p_terrs->size()); i++)
    // // {
    // //     std::cout << (*p_terrs)[i]->getName() << "\n";
    // // }
    // // test 1: print players
    // cout << *player1;
    // cout << *player2;

    // test 2: player's isDoneIssuingOrders
    // cout << player1->isDoneIssuingOrder() << endl;
    // cout << player2->isDoneIssuingOrder() << endl;
    // cout << endl;

    // // test 3: player's getAttackableTerritories()
    // cout << "Player 1 Attackable Territories:\n";
    // for (const auto &pair : player1->getAttackableTerritories())
    // {
    //     std::cout << pair.first << "\n";
    // }
    // cout << "Player 2 Attackable Territories:\n";
    // for (const auto &pair : player2->getAttackableTerritories())
    // {
    //     std::cout << pair.first << "\n";
    // }
    // cout << endl;

    // // test 4: player's getDefendableTerritories()
    // cout << "Player 1 Defendable Territories:\n";
    // for (const auto &pair : player1->getDefendableTerritories())
    // {
    //     std::cout << pair.first << "\n";
    // }
    // cout << "Player 2 Defendable Territories:\n";
    // for (const auto &pair : player2->getDefendableTerritories())
    // {
    //     std::cout << pair.first << "\n";
    // }
    // cout << endl;

    // // test 5: player's toAttack()
    // cout << "Player 1 toAttack Territories:\n";
    // player1->toAttack();
    // cout << "Player 2 toAttack Territories:\n";
    // player2->toAttack();
    // cout << endl;

    // // test 6: player's toDefend()
    // cout << "Player 1 toDefend Territories:\n";
    // player1->toDefend();
    // cout << "Player 2 toDefend Territories:\n";
    // player2->toDefend();
    // cout << endl;

    // // test 7: player deploy armies to defending territories
    // cout << "Player 1 Deploy Order:\n";
    // Deploy *deployOrder1 = player1->deploy(*(player1->getDefending()));
    // cout << *deployOrder1 << endl;
    // cout << "Player 2 Deploy Order:\n";
    // Deploy *deployOrder2 = player2->deploy(*(player2->getDefending()));
    // cout << *deployOrder2 << endl;
    // cout << endl;

    // // test 8: player advance armies to attacking/defending territories
    // cout << "Player 1 Advance Order:\n";
    // Advance *advanceOrder1 = player1->advance(*(player1->getAttacking()), *(player1->getDefending()));
    // if (advanceOrder1 != nullptr)
    // {
    //     cout << *advanceOrder1 << endl;
    // }
    // cout << "Player 2 Advance Order:\n";
    // Advance *advanceOrder2 = player2->advance(*(player2->getAttacking()), *(player2->getDefending()));
    // if (advanceOrder2 != nullptr)
    // {
    //     cout << *advanceOrder2 << endl;
    // }
    // cout << endl;

    // test 9: combine test 7 and 8, and issue card orders
    // engine.issueOrdersPhase();
    // engine.executeOrdersPhase();

    // delete player1;
    // delete player2;
}
void testStartupPhase()
{
    GameEngine engine;
    engine.Attach(std::make_shared<LogObserver>());
    engine.startupPhase();
}

int main()
{
    // testGameStates();
    testStartupPhase();
    // testMainGameLoop();
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}