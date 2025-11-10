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
}


int main() {
    testLoggingObserver();

    return 0;
}
