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
    cout << "============================================" << endl;
    cout << "=== Testing Main Game Loop Functionality ===" << endl;
    cout << "============================================" << endl;
    cout << endl;
    // Set up the environment
    GameEngine engine;

    // 1. Load and validate a map
    std::string file = R"(_62_ small - CASTLE MOONBAT.map)";

    MapLoader loader;

    GameEngine::setGameMap(loader.load(file));

    // 2. Create a deck of cards
    Deck *deck = new Deck();
    // Create random 50 cards and add to deck
    for (int i = 0; i < 50; i++)
    {
        Card *card = createCard(i % 4);
        if (card)
        {
            deck->add(std::shared_ptr<Card>(card));
        }
    }

    GameEngine::setCardDeck(deck);

    // 3. Add players, each of them will be given 5 territories, 10 armies to begin with, and draw 2 cards from the deck
    std::vector<Player *> players;
    Player *player1 = new Player("Minh");
    player1->setReinforcementPool(10);
    player1->getHand()->add(GameEngine::getCardDeck()->draw());
    player1->getHand()->add(GameEngine::getCardDeck()->draw());

    Player *player2 = new Player("Owen");
    player2->setReinforcementPool(10);
    player2->getHand()->add(GameEngine::getCardDeck()->draw());
    player2->getHand()->add(GameEngine::getCardDeck()->draw());

    Player *player3 = new Player("Diana");
    player3->setReinforcementPool(10);
    player3->getHand()->add(GameEngine::getCardDeck()->draw());
    player3->getHand()->add(GameEngine::getCardDeck()->draw());

    players.push_back(player1);
    players.push_back(player2);
    players.push_back(player3);
    GameEngine::setPlayers(players);

    int count = 0;
    // 4. Assign territories to players.
    // For testing the main game loop logic, as well as the winning condition
    // We will assign all territories to player 1 except the last 2 territories to player 2 and 3 respectively
    // This will allow player 1 to conquer all territories and win the game in the main game loop
    for (const auto &pair : GameEngine::getGameMap()->getTerritories())
    {
        if (count == GameEngine::getGameMap()->getTerritories().size() - 2)
        {
            player2->getTerritories()->push_back(pair.second.get());
            pair.second.get()->setPlayer(player2);
        }
        else if (count == GameEngine::getGameMap()->getTerritories().size() - 1)
        {
            player3->getTerritories()->push_back(pair.second.get());
            pair.second.get()->setPlayer(player3);
        }
        else
        {
            player1->getTerritories()->push_back(pair.second.get());
            pair.second.get()->setPlayer(player1);
        }
        count++;
    }
    // 5. Start the main game loop
    engine.mainGameLoop();

    cout << "=== Main Game Loop Test Ended ===" << endl;
    cout << endl
         << endl;

    GameEngine::clearGame();
    // delete player1;
    // delete player2;
    // delete player3;
    // delete deck;
}
void testStartupPhase()
{
    cout << "===========================================" << endl;
    cout << "=== Testing Startup Phase Functionality ===" << endl;
    cout << "===========================================" << endl;
    cout << endl;
    GameEngine engine;
    engine.Attach(std::make_shared<LogObserver>());
    engine.startupPhase();
    cout << "=== Startup Phase Test Ended ===" << endl;
    cout << endl
         << endl;
    GameEngine::clearGame();
}

// int main()
// {
//     // testGameStates();
//     testStartupPhase();
//     // testMainGameLoop();
//     std::cout << "Press Enter to exit...";
//     std::cin.get();
//     return 0;
// }