#include "GameEngine.h"
#include "CommandProcessing.h"
#include "Map.h"
#include <algorithm>
#include <random>
#include <limits>

/*
    ==== Game Engine Class Section ====
*/

// Default Constructor -> Initializes the engine to the start state.
GameEngine::GameEngine() {
    this->currentState = new StartState();
}

// Copy Constructor -> Create a deep copy of another GameEngine
GameEngine::GameEngine(const GameEngine& other) {
    // The following does a deep-copy of the polymorphic state safely
    this->currentState = other.currentState ? other.currentState->clone() : nullptr;
    this->userCommand = other.userCommand;
}

// Assignment Operator
GameEngine& GameEngine::operator=(const GameEngine& other) {
    // Protects against self-assignment 
    if (this == &other) {
        return *this;
    }
    // Clone first (exception-safe) as if clone throws, then "this" remains unchanged
    GameState* newState = other.currentState ? other.currentState->clone() : nullptr;
    // Commit new state only after a successful clone
    delete this->currentState;
    this->currentState = newState;
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
    // Defensive approach:
    // We reject a null "newState" to avoid undefined behavior
    // We add tolerance if ever "currentState" is null
    if (!newState) {
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
//Getters
Map* GameEngine::getGameMap(){
	return this->gameMap.get();
}
vector<Player*> GameEngine::getPlayers(){
	return this->players;
}
//Setters
void GameEngine::setGameMap(unique_ptr<Map> map){
    this->gameMap = std::move(map);
}
void GameEngine::addPlayer(Player* player){
	this->players.push_back(player);
}



string GameEngine::stringToLog(){
    return "New current state is " + currentState->getStateName();
} 

/*
    The following function execute a player command by delegating to the current state.
    Defensive null-guard helps avoid undefined behavior.
*/
void GameEngine::executeCommand(const string& command) {
    if (!currentState) {
        cerr << "[Error] executeCommand: currentState is null" << endl;
        return;
    }
    // Store command and delegate to the current state
    this->userCommand = command;
    this->currentState->processCommand(*this, this->userCommand);
}

string GameEngine::getCurrentStateName() const {
    // Avoids null deref in odd lifecycle moments
    return currentState ? currentState->getStateName() : string("<null>");
}

void GameEngine::startupPhase() {
    std::cout << "//Startup Phase//\n";
	//choose input method:
    std::cout << "read from file (1) or input (2)?";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::unique_ptr<CommandProcessor> processor;
    if (choice != 1 && choice != 2){
        std::cout << "select 1 or 2";
        return;
    }
    if (choice == 1) {
		std::cout << "Enter filename: ";
		std::string filename;
        std::getline(std::cin, filename);
		//make processor adapter
        processor = std::make_unique<FileCommandProcessorAdapter>(filename);
    }
    else {
        processor = std::make_unique<CommandProcessor>();
    }

    Command* command;
    std::string cmnd;
    std::string arg;
   
    while (true) {
        cmnd.clear();
        arg.clear();
       
        command = processor->getCommand();
        //check if empty
        if (command->getCommandText().empty()) {
            std::cout << "No command entered." << std::endl;
			//if reading from file, an empty line means eof
            if (dynamic_cast<FileCommandProcessorAdapter*>(processor.get())) {
                std::cout << "End of file reached. Exiting startup phase.\n";
                break;
            }
            continue;
		}

        std::string commandStr = command->getCommandText();
        //parse commandStr to extract arg if present to seperate arg and command 
        if (commandStr.back() == '>') {
            size_t start = commandStr.find('<');
            size_t end = commandStr.find('>');
            cmnd = commandStr.substr(0, start);
            arg = commandStr.substr(start + 1, end - start - 1);
        }
        else {
            cmnd = commandStr;
        }
        //gamestart is equal to assigncountries from A1
        if (cmnd == "gamestart") {
			cmnd = "assigncountries";
        }
        //verify correct gamestate for command 
        if (!processor->validate(cmnd, this)) {
            cout << "Invalid command for current state: " << this->getCurrentStateName() << endl;
            continue;
        }
        //loadmap command
        if (cmnd == "loadmap") {
            if (arg.empty()) {
                cout << "No map file specified." << endl;
                continue;
            }
            //load map and set map, change gamestate
            auto loader = std::make_unique<MapLoader>();
            if (loader.get()->load(arg) == nullptr) {
                continue;
            }
            this->setGameMap(loader.get()->load(arg));
            this->executeCommand("loadmap");
        }
        //validate map command
        if (cmnd == "validatemap") {
            //change state if map is validated
            if (this->gameMap.get()->validate()) {
                this->executeCommand("validatemap");
            }
            else {
				std::cout << "Map validation failed. Please load a valid map." << std::endl;
            }
        }
        //addplayer command
        if (cmnd == "addplayer") {
            if (arg.empty()) {
                cout << "No player specified." << endl;
                continue;
            }
			//check if playercount is maxed out
            if (this->getPlayers().size() == 6) {
                std::cout << "Maximum number of players (6) reached. Cannot add more players." << std::endl;
                continue;
            }
            //create and add a new player
            this->addPlayer(new Player(arg));
            //state change
            executeCommand("addplayer");
        }
        if (cmnd == "assigncountries") {
            int playerCount = this->getPlayers().size();
            //check if playercount is > 2
            if (playerCount < 2) {
                std::cout << "Add at least 2 players" << std::endl;
                continue;
            }
            
            int c = 0;
            //Goes through map's terretories and assigns them cyclically to players
            for (const auto& pair : this->getGameMap()->getTerritories()) {
                this->getPlayers()[c % playerCount]->addTerritory(pair.second.get());
                c++;
            }
            //determine randomly the order of play (shuffle player vector)
            int pNum = this->getPlayers().size();
            std::random_device rd;
            std::default_random_engine rng(rd());
            std::shuffle(this->getPlayers().begin(), this->getPlayers().end(), rng);

            for (auto p : this->getPlayers()) {
                //give 50 initial army units to players
                p->setReinforcementPool(50);
                std::cout << "Drawing for " << *p;
                //let each player draw 2 initial cards from the deck
                p->getHand()->add(this->cardDeck->draw());
                p->getHand()->add(this->cardDeck->draw());
            }

            //switch the game to the play phase
            this->executeCommand("assigncountries");
			break;
        }
    }
}

void GameEngine::reinforcementPhase() {
    //set all pools to 0, and then terr/3
    for (auto p : this->players) {
        int a = 0;
        a += static_cast<int>(p->getTerritories()->size())/ 3;
        p->setReinforcementPool(a);
    }
	//loop each continent's territories to see if one player owns them all
    for (auto& pair : this->getGameMap()->getContinents()) {
        auto p = pair.second.get()->getTerritories()[0]->getPlayer();
        int i = 1;
        auto terrs = pair.second.get()->getTerritories();
        for (auto* terr : terrs) {
            if (terr->getPlayer()->getName() != p->getName()) {
                break;
            }
			i++; 
            if (i == terrs.size()) {
            p->setReinforcementPool(*(p->getReinforcementPool()) + pair.second.get()->getBonus());
            }
        }
    }
	//ensure minimum 3 armies per player
    for (auto p : this->players) {
		if (*(p->getReinforcementPool()) < 3) {
			p->setReinforcementPool(3);
		}
		std::cout << p << " has " << *(p->getReinforcementPool()) << " armies in reinforcement pool." << std::endl;
    }
}

/*
    ==== Game State Class Section ====
*/

GameState::~GameState() {

}


/*
    ==== Game State Subclasses Section ====
*/

// ==== StartState ====

StartState::StartState() = default;

StartState::StartState(const StartState&) = default;

StartState& StartState::operator=(const StartState&) = default;

StartState::~StartState() = default;

// Polymorphic deep-copy
GameState* StartState::clone() const {
    return new StartState(*this);
}

string StartState::getStateName() const {
    return "start";
}

// The only valid command from "start" is "loadmap" to transition to MapLoadedState
// The other commands are rejected whitout changing state.
void StartState::processCommand(GameEngine& engine, const string& command) {
    if (command == "loadmap") {
		std::cout << "Loading map..." << std::endl;
        engine.transitionTo(new MapLoadedState());
    } else {
        cout << "[Invalid] From 'start' you may only enter: loadmap" << endl;
    }
}

ostream& operator<<(ostream& os, const StartState& s) {
    return os << s.getStateName();
}

// ==== MapLoadedState ====

MapLoadedState::MapLoadedState() = default;

MapLoadedState::MapLoadedState(const MapLoadedState&) = default;

MapLoadedState& MapLoadedState::operator=(const MapLoadedState&) = default;

MapLoadedState::~MapLoadedState() = default;

// Polymorphic deep-copy
GameState* MapLoadedState::clone() const {
    return new MapLoadedState(*this);
}

string MapLoadedState::getStateName() const{
    return "map loaded";
}

// The two valid commands from "map loaded" are "loadmap" to stay in the same state
// and "validatemap" to transition to MapValidatedState.
// The other commands are rejected without changing state.
void MapLoadedState::processCommand(GameEngine& engine, const string& command) {
    if (command == "loadmap") {
        cout << "Reloading... (state unchanged)" << endl;
        return;
    } else if (command == "validatemap") {
		cout << "Validating map..." << endl;
        engine.transitionTo(new MapValidatedState());
    } else {
        cout << "[Invalid] From 'map loaded' you may only enter: loadmap or validatemap" << endl;
    }
}

ostream& operator<<(ostream& os, const MapLoadedState& s) {
    return os << s.getStateName();
}


// ==== MapValidatedState ====

MapValidatedState::MapValidatedState() = default;

MapValidatedState::MapValidatedState(const MapValidatedState&) = default;

MapValidatedState& MapValidatedState::operator=(const MapValidatedState&) = default;

MapValidatedState::~MapValidatedState() = default;

// Polymorphic deep-copy
GameState* MapValidatedState::clone() const {
    return new MapValidatedState(*this);
}

string MapValidatedState::getStateName() const{
    return "map validated";
}

// The only valid command from "map validated" is "addplayer" to transition to PlayersAddedState
// The other commands are rejected without changing state.
void MapValidatedState::processCommand(GameEngine& engine, const string& command) {
    if (command == "addplayer") {
        engine.transitionTo(new PlayersAddedState());
    } else {
        cout << "[Invalid] From 'map validated' you may only enter: addplayer" << endl;
    }
}

ostream& operator<<(ostream& os, const MapValidatedState& s) {
    return os << s.getStateName();
}


// ==== PlayersAddedState ====

PlayersAddedState::PlayersAddedState() = default;

PlayersAddedState::PlayersAddedState(const PlayersAddedState&) = default;

PlayersAddedState& PlayersAddedState::operator=(const PlayersAddedState&) = default;

PlayersAddedState::~PlayersAddedState() = default;

// Polymorphic deep-copy
GameState* PlayersAddedState::clone() const {
    return new PlayersAddedState(*this);
}

string PlayersAddedState::getStateName() const{
    return "players added";
}

// The two valid commands from "players added" are "addplayer" to stay in the same state
// and "assigncountries" to transition to AssignReinforcementState.
// The other commands are rejected without changing state.
void PlayersAddedState::processCommand(GameEngine& engine, const string& command) {
    if (command == "addplayer") {
        cout << "Adding another player... (state unchanged)" << endl;
        return;
    } else if (command == "assigncountries") {
        
        engine.transitionTo(new AssignReinforcementState());
        //this->mainGameLoop();
    } else {
        cout << "[Invalid] From 'players added' you may only enter: addplayer or assigncountries" << endl;
    }
}
ostream& operator<<(ostream& os, const PlayersAddedState& s) {
    return os << s.getStateName();
}


// Main game loop starts from AssignReinforcementState after countries have been assigned

// ==== AssignReinforcementState ====

AssignReinforcementState::AssignReinforcementState() = default;

AssignReinforcementState::AssignReinforcementState(const AssignReinforcementState&) = default;

AssignReinforcementState& AssignReinforcementState::operator=(const AssignReinforcementState&) = default;

AssignReinforcementState::~AssignReinforcementState() = default;

// Polymorphic deep-copy
GameState* AssignReinforcementState::clone() const {
    return new AssignReinforcementState(*this);
}

string AssignReinforcementState::getStateName() const{
    return "assign reinforcement";
}

// The only valid command from "assign reinforcement" is "issueorder" to transition to IssueOrderState
// The other commands are rejected without changing state.
void AssignReinforcementState::processCommand(GameEngine& engine, const string& command) {
    if (command == "issueorder") {
        engine.transitionTo(new IssueOrderState());
    } else {
        cout << "[Invalid] From 'assign reinforcement' you may only enter: issueorder" << endl;
    }
}

ostream& operator<<(ostream& os, const AssignReinforcementState& s) {
    return os << s.getStateName();
}


// ==== IssueOrderState ====

IssueOrderState::IssueOrderState() = default;

IssueOrderState::IssueOrderState(const IssueOrderState&) = default;

IssueOrderState& IssueOrderState::operator=(const IssueOrderState&) = default;

IssueOrderState::~IssueOrderState() = default;

// Polymorphic deep-copy
GameState* IssueOrderState::clone() const {
    return new IssueOrderState(*this);
}

string IssueOrderState::getStateName() const{
    return "issue orders";
}

// The two valid commands from "issue orders" are "issueorder" to stay in the same state
// and "endissueorders" to transition to ExecuteOrderState.
// The other commands are rejected without changing state.
void IssueOrderState::processCommand(GameEngine& engine, const string& command) {
    if (command == "issueorder") {
        cout << "Another order issued... (state unchanged)" << endl;
        return;
    } else if (command == "endissueorders") {
        engine.transitionTo(new ExecuteOrderState());
    } else {
        cout << "[Invalid] From 'issue orders' you may only enter: issueorder or endissueorders" << endl;
    }
}

ostream& operator<<(ostream& os, const IssueOrderState& s) {
    return os << s.getStateName();
}


// ==== ExecuteOrderState ====

ExecuteOrderState::ExecuteOrderState() = default;

ExecuteOrderState::ExecuteOrderState(const ExecuteOrderState&) = default;

ExecuteOrderState& ExecuteOrderState::operator=(const ExecuteOrderState&) = default;

ExecuteOrderState::~ExecuteOrderState() = default;

// Polymorphic deep-copy
GameState* ExecuteOrderState::clone() const {
    return new ExecuteOrderState(*this);
}

string ExecuteOrderState::getStateName() const{
    return "execute orders";
}

// The three valid commands from "execute orders" are "execorder" to stay in the same state,
// "endexecorders" to transition to AssignReinforcementState, and "win" to transition to WinState.
// The other commands are rejected without changing state.
void ExecuteOrderState::processCommand(GameEngine& engine, const string& command) {
    if (command == "execorder") {
        cout << "Another order executed... (state unchanged)" << endl;
        return;
    } else if (command == "endexecorders") {
        engine.transitionTo(new AssignReinforcementState());
    } else if (command == "win") {
        engine.transitionTo(new WinState());
    } else {
        cout << "[Invalid] From 'execute orders' you may only enter: execorder, endexecorders, or win" << endl;
    }
}

ostream& operator<<(ostream& os, const ExecuteOrderState& s) {
    return os << s.getStateName();
}


// Main game loop ends before WinState

// ==== WinState ====

WinState::WinState() = default;

WinState::WinState(const WinState&) = default;

WinState& WinState::operator=(const WinState&) = default;

WinState::~WinState() = default;

// Polymorphic deep-copy
GameState* WinState::clone() const {
    return new WinState(*this);
}

string WinState::getStateName() const{
    return "win";
}

// The two valid commands from "win" are "play" to transition to StartState
// and "end" to transition to EndState.
// The other commands are rejected without changing state.
void WinState::processCommand(GameEngine& engine, const string& command) {
    if (command == "play") {
        engine.transitionTo(new StartState()); // Restart Game
    } else if (command == "end") {
        engine.transitionTo(new EndState()); // Terminate Game (move to endState)
    } else {
        cout << "[Invalid] From 'win' you may only enter: play or end" << endl;
    }
}

ostream& operator<<(ostream& os, const WinState& s) {
    return os << s.getStateName();
}


// ==== EndState ====

EndState::EndState() = default;

EndState::EndState(const EndState&) = default;

EndState& EndState::operator=(const EndState&) = default;

EndState::~EndState() = default;

GameState* EndState::clone() const {
    return new EndState(*this);
}

string EndState::getStateName() const {
    return "end";
}

// End state = end of game => no more state to go
void EndState::processCommand(GameEngine& engine, const string& command) {
    cout << "[Invalid] 'end' is a terminal state. No outgoing transitions." << endl;
}

ostream& operator<<(ostream& os, const EndState& s) {
    return os << s.getStateName();
}
