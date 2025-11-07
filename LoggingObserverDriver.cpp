#include "LoggingObserver.h"
#include "Cards.h"
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"

#include <memory>
#include <unordered_map>

// ================= Test Function =================
void testLoggingObserver() {
    // Clear log file first
    std::ofstream("gamelog.txt", std::ios::trunc).close();

    // Create a LogObserver
    auto logger = std::make_shared<LogObserver>();

    // ===== GameEngine Test =====
    GameEngine engine;
    engine.Attach(logger);
    std::cout << "Testing GameEngine state transitions..." << std::endl;

    engine.executeCommand("loadmap");
    engine.executeCommand("validatemap");
    engine.executeCommand("addplayer");
    engine.executeCommand("assigncountries");
    engine.executeCommand("issueorder");
    engine.executeCommand("endissueorders");
    engine.executeCommand("win");
    engine.executeCommand("end");



    // // ===== CommandProcessor Test =====
    // CommandProcessor cp;
    // cp.Attach(logger);
    // std::cout << "Testing CommandProcessor saveCommand/saveEffect..." << std::endl;

    // Command* c1 = new Command("deploy army");
    // Command* c2 = new Command("advance army");

    // cp.saveCommand(c1);
    // cp.saveCommand(c2);

    // c1->saveEffect("Deployed 5 armies");
    // c2->saveEffect("Advanced to territory X");

    std::cout << "Testing complete. Check gamelog.txt for results." << std::endl;
}

