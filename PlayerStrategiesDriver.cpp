#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "GameEngine.h"
#include <iostream>

using namespace std;
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

void testPlayerStrategies()
{
    cout << "\n===== Testing PlayerStrategies =====\n\n";

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

    deck->shuffle();
    GameEngine::setCardDeck(deck);

    // 3. Add players, each of them will be given 5 territories, 10 armies to begin with, and draw 2 cards from the deck
    std::vector<Player *> players;
    Player *player1 = new Player("Neutral");
    player1->setPlayerStrategy(new NeutralPlayerStrategy(player1));
    player1->setReinforcementPool(10);
    player1->getHand()->add(GameEngine::getCardDeck()->draw());
    player1->getHand()->add(GameEngine::getCardDeck()->draw());

    Player *player2 = new Player("Benevolent");
    player2->setPlayerStrategy(new BenevolentPlayerStrategy(player2));
    player2->setReinforcementPool(10);
    player2->getHand()->add(GameEngine::getCardDeck()->draw());
    player2->getHand()->add(GameEngine::getCardDeck()->draw());

    Player *player3 = new Player("Aggressive");
    player3->setPlayerStrategy(new AggressivePlayerStrategy(player3));
    player3->setReinforcementPool(10);
    player3->getHand()->add(GameEngine::getCardDeck()->draw());
    player3->getHand()->add(GameEngine::getCardDeck()->draw());

    Player *player4 = new Player("Cheater");
    player4->setPlayerStrategy(new CheaterPlayerStrategy(player4));
    player3->setReinforcementPool(10);
    player3->getHand()->add(GameEngine::getCardDeck()->draw());
    player3->getHand()->add(GameEngine::getCardDeck()->draw());

    players.push_back(player1);
    players.push_back(player2);
    players.push_back(player3);
    players.push_back(player4);
    GameEngine::setPlayers(players);

    int count = 0;
    // 4. Assign territories to players.
    // - Cheater has only one territory to begin with to demonstrate cheating behavior
    // - The rest has equal number of territories
    for (const auto &pair : GameEngine::getGameMap()->getTerritories())
    {
        if (count % 4 == 0)
        {
            player1->getTerritories()->push_back(pair.second.get());
            pair.second.get()->setPlayer(player1);
        }
        else if (count % 4 == 1)
        {
            player2->getTerritories()->push_back(pair.second.get());
            pair.second.get()->setPlayer(player2);
        }
        else if (count % 4 == 2)
        {
            player3->getTerritories()->push_back(pair.second.get());
            pair.second.get()->setPlayer(player3);
        }
        else
        {
            // Give only one territory to the cheater at the start
            if (player4->getTerritories()->size() == 0)
            {
                player4->getTerritories()->push_back(pair.second.get());
                pair.second.get()->setPlayer(player4);
            }
            else
            {
                player1->getTerritories()->push_back(pair.second.get());
                pair.second.get()->setPlayer(player1);
            }
        }
        count++;
    }
    // 5. Start the main game loop
    engine.mainGameLoop();

    cout << "=== Player Strategies Test Ended ===" << endl;
    cout << endl
         << endl;

    GameEngine::clearGame();
}