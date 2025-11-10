#ifndef GameEngine_H
#define GameEngine_H

#include <iostream>
#include <string>
// Newly added includes
#include "Player.h"
#include "Map.h"
#include "Cards.h"

using namespace std;

class GameState;

// ==== Game Engine class ====

class GameEngine
{
    // Grant GameState and its children access to GameEngine's private members
    friend class GameState;

public:
    static vector<Player *> players;
    static Map *gameMap;
    static Deck *cardDeck;
    // Default Constructor
    GameEngine();
    // Copy Constructor
    GameEngine(const GameEngine &other);
    // Assignment Operator
    GameEngine &operator=(const GameEngine &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const GameEngine &engine);
    // Destructor
    ~GameEngine();
    // User commands passing
    void executeCommand(const string &command);
    // Current state name getter
    string getCurrentStateName() const;
    // State transition
    void transitionTo(GameState *newState);
    // Getter/setters for players, gameMap and cardDeck
    static vector<Player *> &getPlayers();
    static void setPlayers(const vector<Player *> &players);
    static Map *getGameMap();
    static void setGameMap(Map *map);
    static Deck *getCardDeck();
    static void setCardDeck(Deck *deck);
    static void removePlayer(Player *player);
    void mainGameLoop();

    /* Assignment 2 implementation from here */
    // Move back to private after testing
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();

    // Main game loop method (Part 3)

private:
    GameState *currentState;
    string userCommand;
    // Game Engine should have attributes like: list of players, map, and the deck of cards.
    // The main game loop method. This method will be invoked after we added players.
    // The flow of the main game loop is as follows:
    // 1. Reinforcement Phase: Assign extra armies to each player based on their territories and continents owned (to reinforcement pool)
    // 2. Issue Orders Phase: Each player issues orders in round-robin fashion until all players have finished issuing orders
    // 3. Execute Orders Phase: Each player executes orders in round-robin fashion until all orders from all players have been executed
    //    - Deploy orders are executed first in each round
};

// ==== GameState Base Class ====

class GameState
{
public:
    virtual ~GameState();
    // Pure virtual functions -> no implementation in the base class GameState
    virtual GameState *clone() const = 0;
    // Name of current state as string
    virtual string getStateName() const = 0;
    // String commands processing
    virtual void processCommand(GameEngine &engine, const string &command) = 0;
};

/*
    ==== Game State Subclasses ====
*/

class StartState final : public GameState
{
public:
    // Default Constructor
    StartState();
    // Copy Constructor
    StartState(const StartState &other);
    // Assignment Operator
    StartState &operator=(const StartState &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const StartState &state);
    // Destructor
    ~StartState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

class MapLoadedState final : public GameState
{
public:
    // Default Constructor
    MapLoadedState();
    // Copy Constructor
    MapLoadedState(const MapLoadedState &other);
    // Assignment Operator
    MapLoadedState &operator=(const MapLoadedState &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const MapLoadedState &state);
    // Destructor
    ~MapLoadedState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

class MapValidatedState final : public GameState
{
public:
    // Default Constructor
    MapValidatedState();
    // Copy Constructor
    MapValidatedState(const MapValidatedState &other);
    // Assignment Operator
    MapValidatedState &operator=(const MapValidatedState &other);
    // Steam Insertion Operator
    friend ostream &operator<<(ostream &os, const MapValidatedState &state);
    // Destructor
    ~MapValidatedState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

class PlayersAddedState final : public GameState
{
public:
    // Default Constructor
    PlayersAddedState();
    // Copy Constructor
    PlayersAddedState(const PlayersAddedState &other);
    // Assignment Operator
    PlayersAddedState &operator=(const PlayersAddedState &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const PlayersAddedState &state);
    // Destructor
    ~PlayersAddedState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

class AssignReinforcementState final : public GameState
{
public:
    // Default Constructor
    AssignReinforcementState();
    // Copy Constructor
    AssignReinforcementState(const AssignReinforcementState &other);
    // Assignment Operator
    AssignReinforcementState &operator=(const AssignReinforcementState &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const AssignReinforcementState &state);
    // Destructor
    ~AssignReinforcementState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

class IssueOrderState final : public GameState
{
public:
    // Default Constructor
    IssueOrderState();
    // Copy Constructor
    IssueOrderState(const IssueOrderState &other);
    // Assignment Operator
    IssueOrderState &operator=(const IssueOrderState &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const IssueOrderState &state);
    // Destructor
    ~IssueOrderState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

class ExecuteOrderState final : public GameState
{
public:
    // Default Constructor
    ExecuteOrderState();
    // Constructor with GameEngine reference to invoke executeOrdersPhase
    ExecuteOrderState(GameEngine &engine);
    // Copy Constructor
    ExecuteOrderState(const ExecuteOrderState &other);
    // Assignment Operator
    ExecuteOrderState &operator=(const ExecuteOrderState &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const ExecuteOrderState &state);
    // Destructor
    ~ExecuteOrderState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

class WinState final : public GameState
{
public:
    // Default Constructor
    WinState();
    // Copy Constructor
    WinState(const WinState &other);
    // Assignment Operator
    WinState &operator=(const WinState &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const WinState &state);
    // Destructor
    ~WinState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

class EndState final : public GameState
{
public:
    // Default Constructor
    EndState();
    // Copy Constructor
    EndState(const EndState &other);
    // Assignment Operator
    EndState &operator=(const EndState &other);
    // Stream Insertion Operator
    friend ostream &operator<<(ostream &os, const EndState &state);
    // Destructor
    ~EndState();
    // Clone method
    GameState *clone() const override;
    // Method for this state's specific name
    string getStateName() const override;
    // Method to process commands for this state
    void processCommand(GameEngine &engine, const string &command) override;

private:
};

// Free test function for Game Engine Section
void testGameStates();
void testMainGameLoop();

#endif