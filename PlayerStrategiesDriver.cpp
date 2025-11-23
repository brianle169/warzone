#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include <iostream>

using namespace std;

void testPlayerStrategies() 
{
    cout << "\n===== Testing PlayerStrategies =====\n\n";

    Territory *t1 = new Territory("Canada", nullptr, 100);
    Territory *t2 = new Territory("USA", nullptr, 50);
    Territory *t3 = new Territory("Mexico", nullptr, 70);

    // Making some territories adjacent to eachother
    t1->addEdge(t2);
    t1->addEdge(t3);
    t2->addEdge(t1);
    t2->addEdge(t3);
    t3->addEdge(t1);
    t3->addEdge(t2);
    

    // ----- Create Players -----
    Player* human = new Player("Human");
    PlayerStrategies* human = new HumanPlayerStrategy(human);

    Player* neutral1 = new Player("Neutral1");
    PlayerStrategies* neutral1 = new AggressivePlayerStrategy(neutral1);

    Player* neutral2 = new Player("Neutral2");
    PlayerStrategies* neutral2 = new NeutralPlayerStrategy(neutral2);

    Player* neutral3 = new Player("Neutral3");
    PlayerStrategies* neutral3 = new NeutralPlayerStrategy(neutral3);

    // Give each player territories to make decisions on
    human->addTerritory(t1);
    neutral1->addTerritory(t2);
    neutral2->addTerritory(t3);

    //Requirement 1: Different strategies behave differently 
    cout << "---- Human Strategy (Requires Input) ----\n";
    vector<Territory*> hAtk = human->toAttack();
    vector<Territory*> hDef = human->toDefend();

    cout << "\n---- Neutral Strategy (Does Nothing) ----\n";
    vector<Territory*> nAtk = neutral1->toAttack(); // should return empty
    vector<Territory*> nDef = neutral1->toDefend(); // should return empty

    // Requirement 2: Strategy can change dynamically
    cout << "\n---- Neutral Player Attacked → Becomes Aggressive ----\n";
    cout << "Switching Neutral to Aggressive...\n";
    neutral1->setPlayerStrategy(new AggressivePlayerStrategy(neutral1));

    // Now testing new behavior
    vector<Territory*> newAtk = neutral1->toAttack();
    vector<Territory*> newDef = neutral1->toDefend();

    //Now testing attacking a neutral player and how it changes the neutral player to an aggressive
    Advance* adv = new Advance(human,100,t1,t3);
    adv->execute();


    // Requirement 3: Issue Orders 
    cout << "\n---- Issue Orders (Human asks for input) ----\n";
    human->issueOrder();

    cout << "\n---- Issue Orders (Neutral does nothing) ----\n";
    neutral3->issueOrder();

    // Cleanup
    delete human;
    delete neutral1;
    delete neutral2;

    delete t1;
    delete t2;

}