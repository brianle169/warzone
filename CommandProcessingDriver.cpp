#include "CommandProcessing.h"
#include "GameEngine.h"

/*
    This is the test function for the command processing system
    It tests console input, file input, and command validation
    @param mode is either: "all", "console", "file"
*/

// Helper function to extract the command word from full command string
string extractCommandWord(const string& fullCommand) {
    stringstream ss(fullCommand);
    string cmd;
    ss >> cmd;
    return cmd;
}

void testCommandProcessor(const string& mode) {
    cout << endl << " === Testing Command Processor System === " << endl;

    // Creating a Game Engine obj for validation testing
    GameEngine* engine = new GameEngine();

    // Test 1 -> Read Commands from console
    if (mode == "all" || mode == "console") {
        cout << endl << "=== Test 1: Reading Commands from Console ===" << endl;
        CommandProcessor* consoleCP = new CommandProcessor();

        for (int i = 0; i < 3; i++) {
            cout << "Command " << (i+1) << " [Current state: " << engine->getCurrentStateName() << "]" << endl;
            Command* cmd = consoleCP->getCommand();
            
            if (consoleCP->validate(cmd->getCommandText(), engine)) {
                cout << "Valid -> Executing..." << endl;
                cmd->saveEffect("Valid -> executed successfully");

                string commandWord = extractCommandWord(cmd->getCommandText());
                engine->executeCommand(commandWord);
            } else {
                cout << "Invalid for current state" << endl;
                cmd->saveEffect("Invalid -> rejected");
            }
            cout << "Result: " << *cmd << endl;
            cout << "New state: " << engine->getCurrentStateName() << endl << endl;
        }

        cout << "Console command history:" << endl;
        cout << *consoleCP << "\n" << endl;
        delete consoleCP;
        
        if (mode == "console") {
            delete engine;
            cout << " === Command Processor System Testing Complete === " << endl;
            return;
        }
    }


    // Test 2 -> Read Commands from file
    if (mode == "all" || mode == "file") {
        cout << endl << "=== Test 2: Reading Commands from File ===" << endl;
        
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
                string commandWord = extractCommandWord(cmd->getCommandText());
                engine->executeCommand(commandWord);
            } else {
                cout << "Invalid for current state" << endl;
                cmd->saveEffect("Invalid -> rejected");
            }
            cout << "Result: " << *cmd << endl;
            cout << "New state: " << engine->getCurrentStateName() << endl << endl;
        }
        
        cout << "File command history:" << endl;
        cout << *fileCP << endl << endl;
        delete fileCP;
        
        if (mode == "file") {
            delete engine;
            cout << " === Command Processor System Testing Complete === " << endl;
            return;
        }
    }


    // Test 3 -> Invalid Commands Demonstration (only in "all" mode)
    if (mode == "all") {
        cout << endl << "=== Test 3: Invalid Command Rejection ===" << endl;
        
        delete engine;
        engine = new GameEngine();

        CommandProcessor* testCP = new CommandProcessor();
        
        cout << "Attempting 'validatemap' in START state (should be rejected):" << endl;
        cout << "Current state: " << engine->getCurrentStateName() << endl;
        
        string testCommand1 = "validatemap";
        if (testCP->validate(testCommand1, engine)) {
            cout << "Valid" << endl;
            string commandWord = extractCommandWord(testCommand1);
            engine->executeCommand(commandWord);
        } else {
            cout << "Invalid (expected -> map not loaded)" << endl;
        }
        cout << "State remains: " << engine->getCurrentStateName() << "\n" << endl;
        cout << "Attempting 'loadmap test.map' in START state (should succeed):" << endl;
        cout << "Current state: " << engine->getCurrentStateName() << endl;
        string testCommand2 = "loadmap test.map";
        if (testCP->validate(testCommand2, engine)) {
            cout << "Valid" << endl;
            string commandWord = extractCommandWord(testCommand2);
            engine->executeCommand(commandWord);
        } else {
            cout << "Invalid" << endl;
        }
        cout << "New state: " << engine->getCurrentStateName() << "\n" << endl;
        delete testCP;
    }
    delete engine;
    cout << endl << " === Command Processor System Testing Complete === " << endl;
}


// The following is the driver program that accepts command-line arguments to specify the test mode
int main (int argc, char* argv[]) {
    // No command-line arguments: run all tests
    if (argc < 2) {
        testCommandProcessor("all");
        return 0;
    }
    string mode = argv[1];

    // Console mode: run only Test 1 (console input reading)
    if (mode == "-console") {
        testCommandProcessor("console");
    } 
    // File mode: run only Test 2 (file reading)
    else if (mode == "-file") {
        testCommandProcessor("file");
    } 
    // Invalid command-line option
    else {
        cout << "Usage: ./CommandProcessingDriver [-console | -file]" << endl;
        cout << "No arguments runs all tests." << endl;
        return 1;
    }
    return 0;
}