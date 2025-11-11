#include "LoggingObserver.h"
#include "Cards.h"
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "CommandProcessing.h"

#include <memory>
#include <unordered_map>
#include <iostream>

using namespace std;

// ================= Test Function =================
void testLoggingObserver() {
    // Clear log file first
    std::ofstream("gamelog.txt", std::ios::trunc).close();

    // ===== GameEngine Test =====
    cout << "=== Test: GameEngine State Transitions Logging ===" << endl;

    auto engine = make_shared<GameEngine>();
    auto engineObs = make_shared<LogObserver>();
    engine->Attach(engineObs);

    cout << "Initial state: " << engine->getCurrentStateName() << endl;

    // State transitions
    engine->executeCommand("loadmap");
    engine->executeCommand("validatemap");
    engine->executeCommand("addplayer");

    // ===== CommandProcessor Test =====
    cout << "=== Testing CommandProcessor saveCommand/saveEffect... ===" << endl;

    auto cp = make_unique<CommandProcessor>();
    auto cpObs = make_shared<LogObserver>();
    cp->Attach(cpObs);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    auto c1 = make_shared<Command>(*cp->getCommand());
    auto c2 = make_shared<Command>(*cp->getCommand());

    auto c1Obs = make_shared<LogObserver>();
    auto c2Obs = make_shared<LogObserver>();
    c1->Attach(c1Obs);
    c2->Attach(c2Obs);

    c1->saveEffect("Deployed 5 armies");
    c2->saveEffect("Advanced to territory X");

    // ===== Orders Observer Pattern Test =====
    cout << "=== TESTING ORDERS OBSERVER PATTERN ===" << endl;

    auto alice = make_shared<Player>("Alice");
    auto bob = make_shared<Player>("Bob"); 

    auto t1 = make_shared<Territory>("North", nullptr, 5);
    auto t2 = make_shared<Territory>("South", nullptr, 3);
    t1->setPlayer(alice.get());
    t2->setPlayer(bob.get());

    // Make them adjacent for validation
    t1->addEdge(t2.get());
    t2->addEdge(t1.get());

    auto ordersListObs = make_shared<LogObserver>();
    auto ordersList = alice->getOrdersList(); // pointer to original
    ordersList->Attach(ordersListObs);

    // Created orders as raw pointers directly
    // OrdersList will take ownership and delete them
    Deploy* deployOrder = new Deploy(alice.get(), t1.get(), 10);
    Negotiate* negotiateOrder = new Negotiate(alice.get(), bob.get());

    // Attach observers BEFORE adding to list
    auto deployObs = make_shared<LogObserver>();
    deployOrder->Attach(deployObs);

    auto negotiateObs = make_shared<LogObserver>();
    negotiateOrder->Attach(negotiateObs);

    // Add to the actual list - OrdersList takes ownership
    cout << "\nAdding Deploy order to OrdersList...\n";
    ordersList->addOrder(deployOrder);

    cout << "\nAdding Negotiate order to OrdersList...\n";
    ordersList->addOrder(negotiateOrder);

    // Execute orders (triggers Notify from each Order)
    cout << "\nExecuting Deploy order...\n";
    deployOrder->execute();

    cout << "\nExecuting Negotiate order...\n";
    negotiateOrder->execute();

    cout << "Testing complete. Check gamelog.txt for results." << endl;
}


// int main()  {
//     testLoggingObserver();
//     return 0;
// }