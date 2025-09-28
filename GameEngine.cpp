#include "GameEngine.h"

/*
    ==== Game Engine Class Section ====
*/

// Default Constructor -> Initializes the engine to the start state.
GameEngine::GameEngine() {
    this->currentState = new StartState();
}

// Copy Constructor -> Create a deep copy of another GameEngine
GameEngine::GameEngine(const GameEngine& other) {
    this->currentState = other.currentState->clone();
}

// Assignment Operator
GameEngine& GameEngine::operator=(const GameEngine& other) {
    // Protects against self-assignment 
    if (this == &other) {
        return *this;
    }
    // Deletes current state to free up its memory
    delete this->currentState;
    // Create deep copy of the "other" engine's state
    this->currentState = other.currentState->clone();
    // Copy any other member variables
    this->userCommand = other.userCommand;
    // Return a reference to this object to allow for chained assignments
    return *this;
}

// Stream Insertion Operator
ostream& operator<<(ostream& os, const GameEngine& engine) {
    os << "Current State: " << engine.getCurrentStateName();

    // Return the stream to allow chaining
    return os;
}

// Destructor
GameEngine::~GameEngine() {
    delete this->currentState;
    this->currentState = nullptr;
}

// Transition Member Function
void GameEngine::transitionTo(GameState* newState) {
    cout << "Transitioning from " << this->currentState->getStateName() << " to " << newState->getStateName() << "..." << endl;
    delete this->currentState;
    this->currentState = newState;
}

void GameEngine::executeCommand(const string& command) {
    // Store command
    this->userCommand = command;
    // Tells current state to process this command
    this->currentState->processCommand(*this, this->userCommand);
}

string GameEngine::getCurrentStateName() const {
    return this->currentState->getStateName();
}

/*
    ==== Game State Class Section ====
*/


/*
    ==== Game State Subclasses Section ====
*/