#include "GameEngine.h"
#include "CommandProcessing.h"
#include "LoggingObserver.h"
#include <iostream>

void testTournament() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  TESTING TOURNAMENT MODE" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Clear the log file
    std::ofstream("gamelog.txt", std::ios::trunc).close();
    
    GameEngine engine;
    CommandProcessor cp;
    TournamentConfig config;

    // Test valid command
    // NOTE: Ensure these map files exist in your project directory
    std::string cmd = "tournament -M world.map Australia.map -P Aggressive Benevolent Neutral -G 3 -D 15";
    
    std::cout << "Command: " << cmd << std::endl << std::endl;
    
    if (cp.validateTournamentCommand(cmd, config)) {
        std::cout << " Command validated successfully!" << std::endl;
        std::cout << "\nStarting tournament..." << std::endl;
        
        engine.runTournament(config);
        
        std::cout << "\n Tournament completed!" << std::endl;
        std::cout << "Check gamelog.txt for detailed results." << std::endl;
    } else {
        std::cout << " Invalid tournament command!" << std::endl;
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  TOURNAMENT TEST COMPLETE" << std::endl;
    std::cout << "========================================\n" << std::endl;
}