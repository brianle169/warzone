#include "GameEngine.h"
#include "CommandProcessing.h"
#include "LoggingObserver.h"
#include <iostream>
#include <fstream>

void testTournament() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  TESTING TOURNAMENT MODE" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Clear the log file
    std::ofstream("gamelog.txt", std::ios::trunc).close();
    
    // TEST 1: Valid Command from Console
    std::cout << "\n=== TEST 1: Valid Tournament Command ===" << std::endl;
    GameEngine engine1;
    CommandProcessor cp1;
    TournamentConfig config1;

    std::string validCmd = "tournament -M Australia.map -P Aggressive Benevolent Neutral -G 2 -D 15";
    std::cout << "Command: " << validCmd << std::endl;
    
    if (cp1.validateTournamentCommand(validCmd, config1)) {
        std::cout << " Command validated successfully!" << std::endl;
        std::cout << "\nStarting tournament..." << std::endl;
        engine1.runTournament(config1);
        std::cout << " Tournament completed!" << std::endl;
    } else {
        std::cout << " Validation failed (unexpected)!" << std::endl;
    }
    
    // TEST 2: Invalid Commands
    std::cout << "\n=== TEST 2: Invalid Command Rejection ===" << std::endl;
    
    std::vector<std::string> invalidCommands = {
        "tournament -M world.map -P Aggressive -G 2 -D 15",  // Only 1 strategy
        "tournament -M -P Aggressive Benevolent -G 2 -D 15", // No maps
        "tournament -M world.map -P Human Aggressive -G 2 -D 15", // Human strategy
        "tournament -M world.map -P Aggressive Benevolent -G 10 -D 15", // Too many games
        "tournament -M world.map -P Aggressive Benevolent -G 2 -D 5"  // Too few turns
    };
    
    for (const auto& cmd : invalidCommands) {
        CommandProcessor cp;
        TournamentConfig config;
        std::cout << "\nTesting: " << cmd << std::endl;
        if (cp.validateTournamentCommand(cmd, config)) {
            std::cout << " FAILED: Invalid command was accepted!" << std::endl;
        } else {
            std::cout << " PASSED: Invalid command was rejected" << std::endl;
        }
    }
    
    // TEST 3: Tournament from File
    std::cout << "\n=== TEST 3: Tournament Command from File ===" << std::endl;
    
    // Create test file
    std::ofstream testFile("tournament_test.txt");
    testFile << "tournament -M world.map -P Aggressive Benevolent -G 1 -D 10\n";
    testFile.close();
    
    std::cout << "Created tournament_test.txt" << std::endl;
    std::cout << "To test file input, modify startupPhase to accept this command" << std::endl;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  TOURNAMENT TEST COMPLETE" << std::endl;
    std::cout << "  Check gamelog.txt for results" << std::endl;
    std::cout << "========================================\n" << std::endl;
}