#include "GameEngine.h"

void testGameStates() {
    GameEngine engine;
    cout << "Welcome to Warzone Game Engine version A1!" << endl;
    cout << engine << endl;
    string line;
    while (engine.getCurrentStateName() != "end") {
        cout << "> ";
        // EOF or input error check
        if(!getline(cin, line)) {
            break;
        }
        // Ignore empty user command inputs
        if (line.empty()) {
            continue;
        }
        // Route the user command to the FSM
        engine.executeCommand(line);
        // Show the resulting state after the user command
        cout << engine << endl;
    }
}
void testStartupPhase() {
    GameEngine engine;
    engine.startupPhase();
}

int main() {
    testGameStates();
    testStartupPhase();
    return 0;
}