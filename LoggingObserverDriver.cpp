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

    // Create a LogObserver
    auto logger = std::make_shared<LogObserver>();

    // ===== GameEngine Test =====
    GameEngine engine;
    engine.Attach(logger);
    cout << "Testing GameEngine state transitions..." << std::endl;

    // REPLACE WHEN COMMANDS GET FIXED
    engine.executeCommand("loadmap");
    engine.executeCommand("validatemap");
    engine.executeCommand("addplayer");
    engine.executeCommand("assigncountries");
    engine.executeCommand("issueorder");
    engine.executeCommand("endissueorders");
    engine.executeCommand("win");
    engine.executeCommand("end");


    // ===== CommandProcessor Test =====
    auto cp = make_unique<CommandProcessor>();

    cp->Attach(logger);
    cout << "Testing CommandProcessor saveCommand/saveEffect..." << endl;

    Command* c1 = cp->getCommand();
    Command* c2 = cp->getCommand();

    c1->Attach(logger);
    c2->Attach(logger);

    c1->saveEffect("Deployed 5 armies");
    c2->saveEffect("Advanced to territory X");

    cout << "Testing complete. Check gamelog.txt for results." << endl;
    cout << "=== TESTING ORDERS OBSERVER PATTERN ===" << endl;

    // Create players
    Player* alice = new Player("Alice");
    Player* bob = new Player("Bob");

    // Create dummy territories
    Territory* t1 = new Territory("North", nullptr, 5);
    Territory* t2 = new Territory("South", nullptr, 3);
    t1->setPlayer(alice);
    t2->setPlayer(bob);

    // Make them adjacent for validation
    t1->addEdge(t2);
    t2->addEdge(t1);

    // Create an orders list and attach a log observer
    OrdersList* ol = new OrdersList();
    ol->Attach(logger);

    // Also attach observer to specific orders for demonstration
    Deploy* deployOrder = new Deploy(alice, t1, 10);
    deployOrder->Attach(logger);

    Advance* advanceOrder = new Advance(alice, 5, t1, t2);
    advanceOrder->Attach(logger);

    // Add orders to the list (triggers Notify from OrdersList)
    cout << "\nAdding Deploy order to OrdersList...\n";
    ol->addOrder(deployOrder);

    cout << "\nAdding Advance order to OrdersList...\n";
    ol->addOrder(advanceOrder);

    // Execute orders (triggers Notify from each Order)
    cout << "\nExecuting Deploy order...\n";
    deployOrder->execute();

    cout << "\nExecuting Advance order...\n";
    advanceOrder->execute();

    // Display final orders list
    cout << "\nFinal Orders List:\n" << *ol << endl;

    // Clean up
    delete ol;
    delete alice;
    delete bob;
    delete t1;
    delete t2;
    delete deployOrder;
    delete advanceOrder;   
}


int main() {
    testLoggingObserver();

    return 0;
}
