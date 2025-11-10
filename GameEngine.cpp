#include <iostream>
#include <algorithm>

#include "GameEngine.h"
#include "Player.h"
#include "Cards.h"
#include "Orders.h"
#include "CommandProcessing.h"
#include "Map.h"
#include "LoggingObserver.h"
#include <algorithm>
#include <random>
#include <limits>

using namespace std;
/*
    ==== Game Engine Class Section ====
*/

// Default Constructor -> Initializes the engine to the start state.
GameEngine::GameEngine()
{
    this->currentState = new StartState();
}

// Copy Constructor -> Create a deep copy of another GameEngine
GameEngine::GameEngine(const GameEngine &other)
{
    // The following does a deep-copy of the polymorphic state safely
    this->currentState = other.currentState ? other.currentState->clone() : nullptr;
    this->userCommand = other.userCommand;
}

// Assignment Operator
GameEngine &GameEngine::operator=(const GameEngine &other)
{
    // Protects against self-assignment
    if (this == &other)
    {
        return *this;
    }
    // Clone first (exception-safe) as if clone throws, then "this" remains unchanged
    GameState *newState = other.currentState ? other.currentState->clone() : nullptr;
    // Commit new state only after a successful clone
    delete this->currentState;
    this->currentState = newState;
    // Copy any other member variables
    this->userCommand = other.userCommand;
    // Return a reference to this object to allow for chained assignments
    return *this;
}

// Stream Insertion Operator
ostream &operator<<(ostream &os, const GameEngine &engine)
{
    os << "Current State: " << engine.getCurrentStateName();
    // Return the stream to allow chaining
    return os;
}

// Destructor
GameEngine::~GameEngine()
{
    delete this->currentState;
    this->currentState = nullptr;
}

// Initialization
vector<Player *> GameEngine::players = vector<Player *>();
// Map *GameEngine::gameMap;
unique_ptr<Map> GameEngine::gameMap = nullptr;
Deck *GameEngine::cardDeck = new Deck();

// Static Getter/Setters
vector<Player *> &GameEngine::getPlayers()
{
    return GameEngine::players;
}
void GameEngine::setPlayers(const vector<Player *> &players)
{
    GameEngine::players = players;
}
Map *GameEngine::getGameMap()
{
    return GameEngine::gameMap.get();
}
void GameEngine::setGameMap(unique_ptr<Map> map)
{
    GameEngine::gameMap = std::move(map);
}
Deck *GameEngine::getCardDeck()
{
    return GameEngine::cardDeck;
}
void GameEngine::setCardDeck(Deck *deck)
{
    GameEngine::cardDeck = deck;
}
void GameEngine::removePlayer(Player *player)
{
    for (int i = 0; i < static_cast<int>(GameEngine::getPlayers().size()); i++)
    {
        if (GameEngine::getPlayers()[i] == player)
        {
            GameEngine::getPlayers().erase(GameEngine::getPlayers().begin() + i);
        }
    }
}

// Transition Member Function
void GameEngine::transitionTo(GameState *newState)
{
    // Defensive approach:
    // We reject a null "newState" to avoid undefined behavior
    // We add tolerance if ever "currentState" is null
    if (!newState)
    {
        cerr << "[Error] transitionTo(nullptr) ignored" << endl;
        return;
    }
    const string from = currentState ? currentState->getStateName() : string("<null>");
    const string to = newState->getStateName();

    cout << "Transitioning from " << from << " to " << to << "..." << endl;

    // Release previous owner
    delete currentState;
    // Take ownership of the new state
    currentState = newState;
    Notify(this);
}

void GameEngine::addPlayer(Player *player)
{
    GameEngine::getPlayers().push_back(player);
}

shared_ptr<Card> GameEngine::createCard(const int typeID)
{
    switch (typeID)
    {
    case 0:
        return make_shared<BombCard>();
    case 1:
        return make_shared<BlockadeCard>();
    case 2:
        return make_shared<AirliftCard>();
    case 3:
        return make_shared<DiplomacyCard>();
    default:
        return nullptr;
    }
}

// Create string that will be sent to logger
string GameEngine::stringToLog()
{
    return "New state: " + currentState->getStateName();
}

void GameEngine::clearGame()
{
    // Clear players
    for (Player *player : GameEngine::getPlayers())
    {
        delete player;
    }
    GameEngine::getPlayers().clear();

    // Clear map
    GameEngine::gameMap.reset();

    // Clear deck
    delete GameEngine::cardDeck;
    GameEngine::cardDeck = new Deck();
}

/*
    The following function execute a player command by delegating to the current state.
    Defensive null-guard helps avoid undefined behavior.
*/
void GameEngine::executeCommand(const string &command)
{
    if (!currentState)
    {
        cerr << "[Error] executeCommand: currentState is null" << endl;
        return;
    }
    // Store command and delegate to the current state
    this->userCommand = command;
    this->currentState->processCommand(*this, this->userCommand);
}

string GameEngine::getCurrentStateName() const
{
    // Avoids null deref in odd lifecycle moments
    return currentState ? currentState->getStateName() : string("<null>");
}

void GameEngine::issueOrdersPhase()
{
    // Implementation of the issue orders phase
    // The players are prompted to issue orders in a round-robin fashion
    // until all players have finished issuing their orders.
    cout << "=== Issue Orders Phase Started ===" << endl;
    while (true)
    {
        bool allDone = true;
        for (Player *player : GameEngine::getPlayers())
        {
            if (!player->isDoneIssuingOrder())
            {
                allDone = false;
                player->issueOrder(); // Every player gets to issue only one order per round
            }
        }
        // Once everyone is done issuing orders, we exit the loop.
        // This condition is controlled by the isDoneIssuingOrder() method of each player.
        if (allDone)
        {
            break; // Exit the loop when all players are done issuing orders
        }
    }
    cout << "=== Issue Orders Phase Ended ===" << endl;
    cout << endl;
}

void GameEngine::executeOrdersPhase()
{
    // Implementation of the execute orders phase
    // The players execute their orders in a round-robin fashion
    // until all orders from all players have been executed.
    cout << "=== Execute Orders Phase Started ===" << endl;
    while (true)
    {
        bool allDone = true;
        for (Player *player : GameEngine::getPlayers())
        {
            OrdersList *ordersList = player->getOrdersList();
            if (ordersList && static_cast<int>(ordersList->size()) != 0)
            {
                allDone = false;
                Order *order = ordersList->getOrder(0);
                if (order)
                {
                    cout << "Executing order for player " << player->getName() << ": " << *order << endl;
                    order->execute();
                    ordersList->remove(0); // Remove the executed order from the list
                }
                // Check for winning condition after each order execution
                if (player->hasAllTerritories())
                {
                    cout << "Player " << player->getName() << " has conquered all territories and wins the game!" << endl;
                    // cout << "=== Execute Orders Phase Ended ===" << endl;
                    // cout << "=== Main Game Loop Ended ===" << endl;
                    // Transition to WinState
                    this->transitionTo(new WinState()); // Move to WinState
                    return;
                }
                // And check whether any players will be eliminated
                if (player->getTerritories()->empty())
                {
                    cout << "Player " << player->getName() << " has no territories left and is eliminated from the game." << endl;
                    // Additional logic for player elimination can be added here
                    GameEngine::removePlayer(player);
                }
            }
        }
        if (allDone)
        {
            break; // Exit the loop when all orders have been executed
        }
    }
    cout << "=== Execute Orders Phase Ended ===" << endl;
    cout << endl;
}

// This method will be called to start the main game loop after players have been added
// and the game state is properly setup (distribution of territories, initial armies, play order, get cards from deck, etc.)
// The main game loop continues until a player wins the game.
void GameEngine::mainGameLoop()
{
    // Start from AssignReinforcementState
    cout << "=== Main Game Loop Started ===" << endl;
    while (true)
    {
        // an infinite loop, which will terminate whenever a player wins the game.
        this->transitionTo(new AssignReinforcementState()); // Transition to AssignReinforcementState
        this->reinforcementPhase();                         // Trigger reinforcement phase
        this->transitionTo(new IssueOrderState());          // Transition to IssueOrderState
        this->issueOrdersPhase();                           // Trigger issue orders phase
        for (auto p : GameEngine::getPlayers())
        {
            p->clearState(); // Reset issue order status for the next round
        }
        // this->transitionTo(new ExecuteOrderState()); // Transition to ExecuteOrderState
        // this->executeOrdersPhase();                  // Trigger execute orders phase
        // Check if there's only one player left (winner)
        this->transitionTo(new WinState());
        if (this->currentState->getStateName() == "win")
        {
            cout << "=== Main Game Loop Ended ===" << endl;
            break; // Exit the main game loop if we are already in WinState
        }
        if (GameEngine::getPlayers().size() == 1)
        {
            cout << "Player " << GameEngine::getPlayers()[0]->getName() << " is the last player remaining and wins the game!" << endl;
            cout << "=== Main Game Loop Ended ===" << endl;
            this->transitionTo(new WinState());
            break; // Exit the main game loop
        }
    }
}

void GameEngine::startupPhase()
{
    std::cout << "//Startup Phase//\n";
    // choose input method:
    std::cout << "read from file (1) or input (2)?";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::unique_ptr<CommandProcessor> processor;

    if (choice != 1 && choice != 2)
    {
        std::cout << "select 1 or 2";
        return;
    }
    if (choice == 1)
    {
        std::cout << "Enter filename: ";
        std::string filename;
        std::getline(std::cin, filename);
        // make processor adapter
        processor = std::make_unique<FileCommandProcessorAdapter>(filename);
    }
    else
    {
        processor = std::make_unique<CommandProcessor>();
    }

    for (int i = 0; i < 100; i++)
    {
        shared_ptr<Card> card = createCard(i % 4);
        if (card)
        {
            GameEngine::getCardDeck()->add(card);
        }
    }

    Command *command;
    std::string cmnd;
    std::string arg;

    while (true)
    {
        cmnd.clear();
        arg.clear();

        command = processor->getCommand();
        // check if empty
        if (command->getCommandText().empty())
        {
            std::cout << "No command entered." << std::endl;
            // if reading from file, an empty line means eof
            if (dynamic_cast<FileCommandProcessorAdapter *>(processor.get()))
            {
                std::cout << "End of file reached. Exiting startup phase.\n";
                break;
            }
            continue;
        }

        std::string commandStr = command->getCommandText();
        // parse commandStr to extract arg if present to seperate arg and command
        if (commandStr.back() == '>')
        {
            size_t start = commandStr.find('<');
            size_t end = commandStr.find('>');
            cmnd = commandStr.substr(0, start);
            arg = commandStr.substr(start + 1, end - start - 1);
        }
        else
        {
            cmnd = commandStr;
        }
        // gamestart is equal to assigncountries from A1
        if (cmnd == "gamestart")
        {
            cmnd = "assigncountries";
        }
        // verify correct gamestate for command
        if (!processor->validate(cmnd, this))
        {
            cout << "Invalid command for current state: " << this->getCurrentStateName() << endl;
            continue;
        }
        // loadmap command
        if (cmnd == "loadmap")
        {
            if (arg.empty())
            {
                cout << "No map file specified." << endl;
                continue;
            }
            // load map and set map, change gamestate
            auto loader = std::make_unique<MapLoader>();
            if (loader.get()->load(arg) == nullptr)
            {
                continue;
            }
            GameEngine::setGameMap(loader.get()->load(arg));
            this->executeCommand("loadmap");
        }
        // validate map command
        if (cmnd == "validatemap")
        {
            // change state if map is validated
            if (GameEngine::getGameMap()->validate())
            {
                this->executeCommand("validatemap");
            }
            else
            {
                std::cout << "Map validation failed. Please load a valid map." << std::endl;
            }
        }
        // addplayer command
        if (cmnd == "addplayer")
        {
            if (arg.empty())
            {
                cout << "No player specified." << endl;
                continue;
            }
            // check if playercount is maxed out
            if (GameEngine::getPlayers().size() == 6)
            {
                std::cout << "Maximum number of players (6) reached. Cannot add more players." << std::endl;
                continue;
            }
            // create and add a new player
            GameEngine::addPlayer(new Player(arg));
            // state change
            executeCommand("addplayer");
        }
        if (cmnd == "assigncountries")
        {
            int playerCount = GameEngine::getPlayers().size();
            // check if playercount is > 2
            if (playerCount < 2)
            {
                std::cout << "Add at least 2 players" << std::endl;
                continue;
            }

            int c = 0;
            // Goes through map's terretories and assigns them cyclically to players
            for (const auto &pair : GameEngine::getGameMap()->getTerritories())
            {
                GameEngine::getPlayers()[c % playerCount]->addTerritory(pair.second.get());
                c++;
            }

            for (auto p : GameEngine::getPlayers())
            {
                for (auto terr : *(p->getTerritories()))
                {
                    terr->setPlayer(p);
                }
            }
            // determine randomly the order of play (shuffle player vector)
            int pNum = GameEngine::getPlayers().size();
            std::random_device rd;
            std::default_random_engine rng(rd());
            std::shuffle(GameEngine::getPlayers().begin(), GameEngine::getPlayers().end(), rng);

            for (auto p : GameEngine::getPlayers())
            {
                // give 50 initial army units to players
                p->setReinforcementPool(50);
                std::cout << "Drawing for " << *p;
                // let each player draw 2 initial cards from the deck
                p->getHand()->add(GameEngine::getCardDeck()->draw());
                p->getHand()->add(GameEngine::getCardDeck()->draw());
            }

            // switch the game to the play phase
            this->executeCommand("assigncountries");
        }
        if (cmnd == "end")
        {
            GameEngine::clearGame();
            this->executeCommand("end");
            break;
        }
        if (cmnd == "play")
        {
            GameEngine::clearGame();
            this->executeCommand("play");
            continue;
        }
    }
}

void GameEngine::reinforcementPhase()
{
    cout << "Checkpoint 1" << endl;
    // set all pools to 0, and then terr/3
    for (auto p : GameEngine::getPlayers())
    {
        cout << "Checkpoint 2" << endl;
        int a = 0;
        a += static_cast<int>(p->getTerritories()->size()) / 3;
        p->setReinforcementPool(p->getReinforcementPool() + a);
    }
    // loop each continent's territories to see if one player owns them all
    for (auto &pair : GameEngine::getGameMap()->getContinents())
    {
        cout << "Checkpoint 3" << endl;
        auto p = pair.second.get()->getTerritories()[0]->getPlayer();
        int i = 1;
        auto terrs = pair.second.get()->getTerritories();
        for (auto *terr : terrs)
        {
            if (terr->getPlayer()->getName() != p->getName())
            {
                break;
            }
            i++;
            if (i == terrs.size())
            {
                p->setReinforcementPool((p->getReinforcementPool()) + pair.second.get()->getBonus());
            }
        }
    }
    // ensure minimum 3 armies per player
    for (auto p : GameEngine::getPlayers())
    {
        cout << "Checkpoint 4" << endl;
        if ((p->getReinforcementPool()) < 3)
        {
            p->setReinforcementPool(3);
        }
        std::cout << p << " has " << (p->getReinforcementPool()) << " armies in reinforcement pool." << std::endl;
    }
}

/*
    ==== Game State Class Section ====
*/

GameState::~GameState()
{
}

/*
    ==== Game State Subclasses Section ====
*/

// ==== StartState ====

StartState::StartState() = default;

StartState::StartState(const StartState &) = default;

StartState &StartState::operator=(const StartState &) = default;

StartState::~StartState() = default;

// Polymorphic deep-copy
GameState *StartState::clone() const
{
    return new StartState(*this);
}

string StartState::getStateName() const
{
    return "start";
}

// The only valid command from "start" is "loadmap" to transition to MapLoadedState
// The other commands are rejected whitout changing state.
void StartState::processCommand(GameEngine &engine, const string &command)
{
    if (command == "loadmap")
    {
        std::cout << "Loading map..." << std::endl;
        engine.transitionTo(new MapLoadedState());
    }
    else
    {
        cout << "[Invalid] From 'start' you may only enter: loadmap" << endl;
    }
}

ostream &operator<<(ostream &os, const StartState &s)
{
    return os << s.getStateName();
}

// ==== MapLoadedState ====

MapLoadedState::MapLoadedState() = default;

MapLoadedState::MapLoadedState(const MapLoadedState &) = default;

MapLoadedState &MapLoadedState::operator=(const MapLoadedState &) = default;

MapLoadedState::~MapLoadedState() = default;

// Polymorphic deep-copy
GameState *MapLoadedState::clone() const
{
    return new MapLoadedState(*this);
}

string MapLoadedState::getStateName() const
{
    return "map loaded";
}

// The two valid commands from "map loaded" are "loadmap" to stay in the same state
// and "validatemap" to transition to MapValidatedState.
// The other commands are rejected without changing state.
void MapLoadedState::processCommand(GameEngine &engine, const string &command)
{
    if (command == "loadmap")
    {
        cout << "Reloading... (state unchanged)" << endl;
        return;
    }
    else if (command == "validatemap")
    {
        cout << "Validating map..." << endl;
        engine.transitionTo(new MapValidatedState());
    }
    else
    {
        cout << "[Invalid] From 'map loaded' you may only enter: loadmap or validatemap" << endl;
    }
}

ostream &operator<<(ostream &os, const MapLoadedState &s)
{
    return os << s.getStateName();
}

// ==== MapValidatedState ====

MapValidatedState::MapValidatedState() = default;

MapValidatedState::MapValidatedState(const MapValidatedState &) = default;

MapValidatedState &MapValidatedState::operator=(const MapValidatedState &) = default;

MapValidatedState::~MapValidatedState() = default;

// Polymorphic deep-copy
GameState *MapValidatedState::clone() const
{
    return new MapValidatedState(*this);
}

string MapValidatedState::getStateName() const
{
    return "map validated";
}

// The only valid command from "map validated" is "addplayer" to transition to PlayersAddedState
// The other commands are rejected without changing state.
void MapValidatedState::processCommand(GameEngine &engine, const string &command)
{
    if (command == "addplayer")
    {
        engine.transitionTo(new PlayersAddedState());
    }
    else
    {
        cout << "[Invalid] From 'map validated' you may only enter: addplayer" << endl;
    }
}

ostream &operator<<(ostream &os, const MapValidatedState &s)
{
    return os << s.getStateName();
}

// ==== PlayersAddedState ====

PlayersAddedState::PlayersAddedState() = default;

PlayersAddedState::PlayersAddedState(const PlayersAddedState &) = default;

PlayersAddedState &PlayersAddedState::operator=(const PlayersAddedState &) = default;

PlayersAddedState::~PlayersAddedState() = default;

// Polymorphic deep-copy
GameState *PlayersAddedState::clone() const
{
    return new PlayersAddedState(*this);
}

string PlayersAddedState::getStateName() const
{
    return "players added";
}

// The two valid commands from "players added" are "addplayer" to stay in the same state
// and "assigncountries" to transition to AssignReinforcementState.
// The other commands are rejected without changing state.
void PlayersAddedState::processCommand(GameEngine &engine, const string &command)
{
    if (command == "addplayer") // we add more player in here
    {
        cout << "Adding another player... (state unchanged)" << endl;
        return;
    }
    else if (command == "assigncountries")
    {
        engine.mainGameLoop();
    }
    else
    {
        cout << "[Invalid] From 'players added' you may only enter: addplayer or assigncountries" << endl;
    }
}
ostream &operator<<(ostream &os, const PlayersAddedState &s)
{
    return os << s.getStateName();
}

// Main game loop starts from AssignReinforcementState after countries have been assigned

// ==== AssignReinforcementState ====

AssignReinforcementState::AssignReinforcementState() = default;

AssignReinforcementState::AssignReinforcementState(const AssignReinforcementState &) = default;

AssignReinforcementState &AssignReinforcementState::operator=(const AssignReinforcementState &) = default;

AssignReinforcementState::~AssignReinforcementState() = default;

// Polymorphic deep-copy
GameState *AssignReinforcementState::clone() const
{
    return new AssignReinforcementState(*this);
}

string AssignReinforcementState::getStateName() const
{
    return "assign reinforcement";
}

// The only valid command from "assign reinforcement" is "issueorder" to transition to IssueOrderState
// The other commands are rejected without changing state.
void AssignReinforcementState::processCommand(GameEngine &engine, const string &command)
{
    if (command == "issueorder")
    {
        engine.transitionTo(new IssueOrderState());
    }
    else
    {
        cout << "[Invalid] From 'assign reinforcement' you may only enter: issueorder" << endl;
    }
}

ostream &operator<<(ostream &os, const AssignReinforcementState &s)
{
    return os << s.getStateName();
}

// ==== IssueOrderState ====

IssueOrderState::IssueOrderState() = default;

IssueOrderState::IssueOrderState(const IssueOrderState &) = default;

IssueOrderState &IssueOrderState::operator=(const IssueOrderState &) = default;

IssueOrderState::~IssueOrderState() = default;

// Polymorphic deep-copy
GameState *IssueOrderState::clone() const
{
    return new IssueOrderState(*this);
}

string IssueOrderState::getStateName() const
{
    return "issue orders";
}

// The two valid commands from "issue orders" are "issueorder" to stay in the same state
// and "endissueorders" to transition to ExecuteOrderState.
// The other commands are rejected without changing state.
void IssueOrderState::processCommand(GameEngine &engine, const string &command)
{
    if (command == "issueorder")
    {
        cout << "Another order issued... (state unchanged)" << endl;
        return;
    }
    else if (command == "endissueorders")
    {
        engine.transitionTo(new ExecuteOrderState());
    }
    else
    {
        cout << "[Invalid] From 'issue orders' you may only enter: issueorder or endissueorders" << endl;
    }
}

ostream &operator<<(ostream &os, const IssueOrderState &s)
{
    return os << s.getStateName();
}

// ==== ExecuteOrderState ====

ExecuteOrderState::ExecuteOrderState() = default;

ExecuteOrderState::ExecuteOrderState(const ExecuteOrderState &) = default;

ExecuteOrderState &ExecuteOrderState::operator=(const ExecuteOrderState &) = default;

ExecuteOrderState::~ExecuteOrderState() = default;

// Polymorphic deep-copy
GameState *ExecuteOrderState::clone() const
{
    return new ExecuteOrderState(*this);
}

string ExecuteOrderState::getStateName() const
{
    return "execute orders";
}

// The three valid commands from "execute orders" are "execorder" to stay in the same state,
// "endexecorders" to transition to AssignReinforcementState, and "win" to transition to WinState.
// The other commands are rejected without changing state.
void ExecuteOrderState::processCommand(GameEngine &engine, const string &command)
{
    if (command == "execorder")
    {
        cout << "Another order executed... (state unchanged)" << endl;
        return;
    }
    else if (command == "endexecorders")
    {
        engine.transitionTo(new AssignReinforcementState());
    }
    else if (command == "win")
    {
        engine.transitionTo(new WinState());
    }
    else
    {
        cout << "[Invalid] From 'execute orders' you may only enter: execorder, endexecorders, or win" << endl;
    }
}

ostream &operator<<(ostream &os, const ExecuteOrderState &s)
{
    return os << s.getStateName();
}

// Main game loop ends before WinState

// ==== WinState ====

WinState::WinState() = default;

WinState::WinState(const WinState &) = default;

WinState &WinState::operator=(const WinState &) = default;

WinState::~WinState() = default;

// Polymorphic deep-copy
GameState *WinState::clone() const
{
    return new WinState(*this);
}

string WinState::getStateName() const
{
    return "win";
}

// The two valid commands from "win" are "play" to transition to StartState
// and "end" to transition to EndState.
// The other commands are rejected without changing state.
void WinState::processCommand(GameEngine &engine, const string &command)
{
    if (command == "play")
    {
        engine.transitionTo(new StartState()); // Restart Game
        engine.startupPhase();                 // Start new game
    }
    else if (command == "end")
    {
        engine.transitionTo(new EndState()); // Terminate Game (move to endState)
    }
    else
    {
        cout << "[Invalid] From 'win' you may only enter: play or end" << endl;
    }
}

ostream &operator<<(ostream &os, const WinState &s)
{
    return os << s.getStateName();
}

// ==== EndState ====

EndState::EndState() = default;

EndState::EndState(const EndState &) = default;

EndState &EndState::operator=(const EndState &) = default;

EndState::~EndState() = default;

GameState *EndState::clone() const
{
    return new EndState(*this);
}

string EndState::getStateName() const
{
    return "end";
}

// End state = end of game => no more state to go
void EndState::processCommand(GameEngine &engine, const string &command)
{
    cout << "[Invalid] 'end' is a terminal state. No outgoing transitions." << endl;
}

ostream &operator<<(ostream &os, const EndState &s)
{
    return os << s.getStateName();
}
