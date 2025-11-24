#include "CommandProcessing.h"
#include "GameEngine.h"
#include "LoggingObserver.h"
#include <sstream>
#include <set>
#include <algorithm>


/*
    === Tournament Config Struct Implementations ===
*/

// Constructor
TournamentConfig::TournamentConfig() : mapFiles(new vector<string>()), playerStrategies(new vector<string>()), numberOfGames(0), maxTurns(0) {}

// Copy Constructor
TournamentConfig::TournamentConfig(const TournamentConfig& other) : mapFiles(new vector<string>(*other.mapFiles)), playerStrategies(new vector<string>(*other.playerStrategies)), numberOfGames(other.numberOfGames), maxTurns(other.maxTurns) {}

// Assignment Operator
TournamentConfig& TournamentConfig::operator=(const TournamentConfig& other) {
    if (this != &other) {
        delete mapFiles;
        delete playerStrategies;
        mapFiles = new vector<string>(*other.mapFiles);
        playerStrategies = new vector<string>(*other.playerStrategies);
        numberOfGames = other.numberOfGames;
        maxTurns = other.maxTurns;
    }
    return *this;
}

// Destructor
TournamentConfig::~TournamentConfig() {
    delete mapFiles;
    delete playerStrategies;
}


/*
    === Command Class Implementation ===
    It manages individual commands with their text and effects
*/

// Default Constructor
Command::Command()
{
    commandText = new string("");
    effect = new string("");
}
// Constructor with parameter
Command::Command(const string &cmd)
{
    commandText = new string(cmd);
    effect = new string("");
}
// Copy Constructor
Command::Command(const Command &other)
{
    commandText = new string(*other.commandText);
    effect = new string(*other.effect);
}
// Destructor
Command::~Command()
{
    delete commandText;
    delete effect;
}
// Assignment Operator
Command &Command::operator=(const Command &other)
{
    if (this != &other)
    {
        delete commandText;
        delete effect;

        commandText = new string(*other.commandText);
        effect = new string(*other.effect);
    }
    return *this;
}
// Stream Insertion Operator
ostream &operator<<(ostream &os, const Command &cmd)
{
    os << "Command: '" << *cmd.commandText << "'";
    if (!cmd.effect->empty())
    {
        os << " | Effect: " << *cmd.effect;
    }
    return os;
}
// Getters
string Command::getCommandText() const
{
    return *commandText;
}
string Command::getEffect() const
{
    return *effect;
}
// Save the effect of a command execution
void Command::saveEffect(const string &eff)
{
    *effect = eff;
    Notify(this);
}

// Create string that will be sent to logger
string Command::stringToLog()
{
    return "Command executed: " + *commandText + " | Effect: " + *effect;
}

/*
    === CommandProcessor Class Implementation ===
    It processes and validates commands from console input
*/

// Default Constructor
CommandProcessor::CommandProcessor()
{
    commands = new vector<Command *>();
}

// Copy Constructor
CommandProcessor::CommandProcessor(const CommandProcessor &other)
{
    commands = new vector<Command *>();
    for (Command *cmd : *other.commands)
    {
        commands->push_back(new Command(*cmd));
    }
}

// Destructor
CommandProcessor::~CommandProcessor()
{
    for (Command *cmd : *commands)
    {
        delete cmd;
    }
    delete commands;
}

// Assignment Operator
CommandProcessor &CommandProcessor::operator=(const CommandProcessor &other)
{
    if (this != &other)
    {
        for (Command *cmd : *commands)
        {
            delete cmd;
        }
        commands->clear();

        for (Command *cmd : *other.commands)
        {
            commands->push_back(new Command(*cmd));
        }
    }
    return *this;
}

// String Insertion Operator
ostream &operator<<(ostream &os, const CommandProcessor &cp)
{
    os << "CommandProcessor with " << cp.commands->size() << " command(s):";
    for (const Command *cmd : *cp.commands)
    {
        os << "\n - " << *cmd;
    }
    return os;
}

// Read command from console
string CommandProcessor::readCommand()
{
    string input;
    cout << "\nEnter command: ";
    getline(cin, input);
    return input;
}

// Save command to command collection
void CommandProcessor::saveCommand(Command *cmd)
{
    if (cmd)
    {
        commands->push_back(cmd);
        Notify(this);
    }
}

// Get command
Command *CommandProcessor::getCommand()
{
    string cmdText = readCommand();
    Command *cmd = new Command(cmdText);
    saveCommand(cmd);
    return cmd;
}

// Validate command based on game state
bool CommandProcessor::validate(const string &command, GameEngine *engine)
{
    if (!engine)
    {
        return false;
    }
    string currentState = engine->getCurrentStateName();

    // Parse command by extracting the first word
    stringstream ss(command);
    string cmd;
    ss >> cmd;

    if (currentState == "start")
    {
        return (cmd == "loadmap");
    }
    else if (currentState == "map loaded")
    {
        return (cmd == "loadmap" || cmd == "validatemap");
    }
    else if (currentState == "map validated")
    {
        return (cmd == "addplayer");
    }
    else if (currentState == "players added")
    {
        return (cmd == "addplayer" || cmd == "assigncountries");
    }
    else if (currentState == "assign reinforcement")
    {
        return (cmd == "issueorder");
    }
    else if (currentState == "issue orders")
    {
        return (cmd == "issueorder" || cmd == "endissueorders");
    }
    else if (currentState == "execute orders")
    {
        return (cmd == "execorder" || cmd == "endexecorders" || cmd == "win");
    }
    else if (currentState == "win")
    {
        return (cmd == "play" || cmd == "end");
    }
    return false;
}

// Validate tournament command
bool CommandProcessor::validateTournamentCommand(const std::string &commandStr, TournamentConfig &config)
{
    std::stringstream ss(commandStr);
    std::string token;
    std::vector<std::string> tokens;

    // Tokenize the command string
    while (ss >> token)
    {
        tokens.push_back(token);
    }

    // Check if command starts with "tournament"
    if (tokens.empty() || tokens[0] != "tournament")
    {
        return false;
    }

    // Parse flags and their values
    for (size_t i = 1; i < tokens.size(); ++i)
    {
        if (tokens[i] == "-M")
        {
            // Collect all map files until next flag
            i++;
            while (i < tokens.size() && tokens[i][0] != '-')
            {
                config.mapFiles->push_back(tokens[i]);
                i++;
            }
            i--; // Adjust for outer loop increment
        }
        else if (tokens[i] == "-P")
        {
            // Collect all player strategies until next flag
            i++;
            while (i < tokens.size() && tokens[i][0] != '-')
            {
                config.playerStrategies->push_back(tokens[i]);
                i++;
            }
            i--;
        }
        else if (tokens[i] == "-G")
        {
            if (i + 1 < tokens.size())
            {
                try
                {
                    config.numberOfGames = std::stoi(tokens[i + 1]);
                }
                catch (...)
                {
                    return false;
                }
                i++;
            }
        }
        else if (tokens[i] == "-D")
        {
            if (i + 1 < tokens.size())
            {
                try
                {
                    config.maxTurns = std::stoi(tokens[i + 1]);
                }
                catch (...)
                {
                    return false;
                }
                i++;
            }
        }
    }

    // Validation Rules

    // 1. Maps: 1-5 distinct map files
    std::set<std::string> uniqueMaps(config.mapFiles->begin(), config.mapFiles->end());
    if (uniqueMaps.size() < 1 || uniqueMaps.size() > 5)
    {
        std::cout << "Error: Must have 1-5 distinct map files" << std::endl;
        return false;
    }

    // 2. Strategies: 2-4 distinct player strategies
    std::set<std::string> uniqueStrats(config.playerStrategies->begin(), config.playerStrategies->end());
    if (uniqueStrats.size() < 2 || uniqueStrats.size() > 4)
    {
        std::cout << "Error: Must have 2-4 distinct player strategies" << std::endl;
        return false;
    }

    // Validate strategy names against existing classes
    for (const auto &stratName : uniqueStrats)
    {
        if (stratName != "Aggressive" &&
            stratName != "Benevolent" &&
            stratName != "Neutral" &&
            stratName != "Cheater")
        {
            std::cout << "Error: Invalid strategy name: " << stratName << std::endl;
            return false;
        }
    }

    // 3. Games: 1-5
    if (config.numberOfGames < 1 || config.numberOfGames > 5)
    {
        std::cout << "Error: Number of games must be between 1 and 5" << std::endl;
        return false;
    }

    // 4. Max Turns: 10-50
    if (config.maxTurns < 10 || config.maxTurns > 50)
    {
        std::cout << "Error: Max turns must be between 10 and 50" << std::endl;
        return false;
    }

    return true;
}

// Get command list
const vector<Command *> *CommandProcessor::getCommandList() const
{
    return commands;
}

// Create string that will be sent to logger
string CommandProcessor::stringToLog()
{
    if (commands->empty())
    {
        return "CommandProcessor: no commands yet.";
    }
    Command *lastCommand = commands->back();
    return "Command saved: " + lastCommand->getCommandText();
}

/*
    === FileLineReader Class Implementation
    Reads lines from a text file
*/

// Default Constructor
FileLineReader::FileLineReader()
{
    filename = new string("");
    fileStream = nullptr;
}

// Constructor with parameter
FileLineReader::FileLineReader(const string &file)
{
    filename = new string(file);
    fileStream = new ifstream(*filename);
    if (!fileStream->is_open())
    {
        cerr << "Error: Could not open the wanted file " << *filename << endl;
        delete fileStream;
        fileStream = nullptr;
    }
}

// Copy Constructor
FileLineReader::FileLineReader(const FileLineReader &other)
{
    filename = new string(*other.filename);
    if (!filename->empty())
    {
        fileStream = new ifstream(*filename);
    }
    else
    {
        fileStream = nullptr;
    }
}

// Destructor
FileLineReader::~FileLineReader()
{
    if (fileStream)
    {
        if (fileStream->is_open())
        {
            fileStream->close();
        }
        delete fileStream;
    }
    delete filename;
}

// Assignment Operator
FileLineReader &FileLineReader::operator=(const FileLineReader &other)
{
    if (this != &other)
    {
        if (fileStream)
        {
            if (fileStream->is_open())
            {
                fileStream->close();
            }
            delete fileStream;
        }
        delete filename;

        filename = new string(*other.filename);
        if (!filename->empty())
        {
            fileStream = new ifstream(*filename);
        }
        else
        {
            fileStream = nullptr;
        }
    }
    return *this;
}

// Stream Insertion Operator
ostream &operator<<(ostream &os, const FileLineReader &reader)
{
    os << "FileLineReader for file: " << *reader.filename;
    return os;
}

// Read line from file
string FileLineReader::readLineFromFile()
{
    string line;
    if (fileStream && fileStream->is_open() && getline(*fileStream, line))
    {
        return line;
    }
    return "";
}

/*
    === FileCommandProcessorAdapter Class Implementation
    It adapts CommandProcessor to read from files using FileLineReader class
*/

// Default Constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter() : CommandProcessor(), flr(nullptr)
{
}

// Constructor with parameter
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string &filename) : CommandProcessor()
{
    flr = new FileLineReader(filename);
}

// Copy Constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &other) : CommandProcessor(other)
{
    if (other.flr)
    {
        flr = new FileLineReader(*other.flr);
    }
    else
    {
        flr = nullptr;
    }
}

// Destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    delete flr;
}

// Assignment Operator
FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &other)
{
    if (this != &other)
    {
        CommandProcessor::operator=(other);
        delete flr;

        if (other.flr)
        {
            flr = new FileLineReader(*other.flr);
        }
        else
        {
            flr = nullptr;
        }
    }
    return *this;
}

// Stream Insertion Operator
ostream &operator<<(ostream &os, const FileCommandProcessorAdapter &adapter)
{
    os << "FileCommandProcessorAdapter";
    if (adapter.flr)
    {
        os << " using " << *adapter.flr;
    }
    return os;
}

// Override readCommand method to read from a file instead of the console
string FileCommandProcessorAdapter::readCommand()
{
    if (flr)
    {
        string line = flr->readLineFromFile();
        if (!line.empty())
        {
            cout << "Read from file: " << line << endl;
            return line;
        }
    }
    return "";
}

// Create string that will be sent to logger
string FileCommandProcessorAdapter::stringToLog()
{
    if (!flr)
    {
        return "";
    }

    return "";
}
