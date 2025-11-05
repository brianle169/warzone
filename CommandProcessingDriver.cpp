#include "CommandProcessor.h"
#include "GameEngine.h"

// Part 1 Testing Free Function
void testCommandProcessor() {
    cout << endl << " === Testing Command Processor System === " << endl;

    // Creating a Game Egnine obj for validation testing
    GameEngine* engine = new GameEngine();


    // Test 1 -> Read Commands from console
    cout << "Reading Commands from Console: " << endl;
    CommandProcessor* consoleCP = new CommandProcessor();

    // Read 2 commands from the console
    for (int i = 0; i < 3; i++) {
        cout << "Command " << (i+1) << " [Current state: " << engine->getCurrentStateName() << "]" << endl;
        Command* cmd = consoleCP->getCommand();
        
        if (consoleCP->validate(cmd->getCommandText(), engine)) {
            cout << "Valid -> Executing..." << endl;
            cmd->saveEffect("Valid -> executed successfully");
            engine->executeCommand(cmd->getCommandText());
        } else {
            cout << "Invalid for current state" << endl;
            cmd->saveEffect("Invalid -> rejected");
        }
        cout << "Result: " << *cmd << endl;
        cout << "New state: " << engine->getCurrentStateName() << endl << endl;
    }

    cout << "Console command history:" << endl;
    cout << *consoleCP << "\n" << endl;


    // Test 2 -> Read Commands from file
    cout << endl << "Reading Commands from file: " << endl;
    // Create the test file
    ofstream file("commands.txt");
    file << "loadmap world.map\n";
    file << "validatemap\n";
    file << "addplayer Alice\n";
    file << "addplayer Bob\n";
    file << "assigncountries\n";
    file.close();
    cout << "Created 'commands.txt' with 5 commands\n" << endl;

    // Reset the game engine to the start state
    delete engine;
    engine = new GameEngine();

    // Read from the file using the adapter class
    FileCommandProcessorAdapter* fileCP = new FileCommandProcessorAdapter("commands.txt");

    Command* cmd;
    int cmdNum = 1;
    while ((cmd = fileCP->getCommand()) && !cmd->getCommandText().empty()) {
        cout << "Command " << cmdNum++ << " [State: " << engine->getCurrentStateName() << "]" << endl;
        
        if (fileCP->validate(cmd->getCommandText(), engine)) {
            cout << "Valid -> Executing..." << endl;
            cmd->saveEffect("Valid -> executed successfully");
            engine->executeCommand(cmd->getCommandText());
        } else {
            cout << "Invalid for current state" << endl;
            cmd->saveEffect("Invalid -> rejected");
        }
        cout << "Result: " << *cmd << endl;
        cout << "New state: " << engine->getCurrentStateName() << endl << endl;
    }
    cout << "File command history:" << endl;
    cout << *fileCP << endl << endl;


    // Test 3 -> Invalid Commands Demonstration
    delete engine;
    engine = new GameEngine();

    CommandProcessor* testCP = new CommandProcessor();
    
    cout << "Attempting 'validatemap' in START state (should be rejected):" << endl;
    cout << "Current state: " << engine->getCurrentStateName() << endl;
    
    // Create command and store it via the getCommand simulation
    string testCommand1 = "validatemap";
    if (testCP->validate(testCommand1, engine)) {
        cout << "Valid" << endl;
        engine->executeCommand(testCommand1);
    } else {
        cout << "Invalid (expected -> map not loaded)" << endl;
    }
    cout << "State remains: " << engine->getCurrentStateName() << "\n" << endl;
    cout << "Attempting 'loadmap test.map' in START state (should succeed):" << endl;
    cout << "Current state: " << engine->getCurrentStateName() << endl;
    string testCommand2 = "loadmap test.map";
    if (testCP->validate(testCommand2, engine)) {
        cout << "Valid" << endl;
        engine->executeCommand(testCommand2);
    } else {
        cout << "Invalid" << endl;
    }
    cout << "New state: " << engine->getCurrentStateName() << "\n" << endl;

    // A bit of cleanup
    delete consoleCP;
    delete fileCP;
    delete testCP;
    delete engine;
    
    cout << endl << " === Command Processor System Testing Complete === " << endl;
}

int main () {
    testCommandProcessor();
    return 0;
}