#include <iostream>
#include <string>
using namespace std;

#ifndef GameEngine_H
#define GameEngine_H

enum class StateID {
    // Startup phase
    Start,
    MapLoaded,
    MapValidated,
    PlayersAdded,

    // Play phase
    AssignReinforcement,
    IssueOrder,
    ExecuteOrder,
    Win
};

class GameEngine {
    public:
        GameEngine();
        ~GameEngine();

    private:
        StateID current_;
};


#endif