#include "CommandProcessing.h"
#include "GameEngine.h"
#include "LoggingObserver.h"



/*
    === Command Class Implementation ===
    It manages individual commands with their text and effects
*/

// Default Constructor
Command::Command(){
    commandText = new string("");
    effect = new string("");
}
// Constructor with parameter
Command::Command(const string& cmd){
    commandText = new string(cmd);
    effect = new string("");
}
// Copy Constructor
Command::Command(const Command& other){
    commandText = new string(*other.commandText);
    effect = new string(*other.effect);
}
// Destructor
Command::~Command(){
    delete commandText;
    delete effect;
}
// Assignment Operator
Command& Command::operator=(const Command& other){
    if (this != &other) {
        delete commandText;
        delete effect;

        commandText = new string(*other.commandText);
        effect = new string(*other.effect);
    }
    return *this;
}
// Stream Insertion Operator
ostream& operator<<(ostream& os, const Command& cmd) {
    os << "Command: '" << *cmd.commandText << "'";
    if (!cmd.effect->empty()){
        os << " | Effect: " << *cmd.effect;
    }
    return os;
}
// Getters
string Command::getCommandText() const {
    return *commandText;
}
string Command::getEffect() const {
    return *effect;
}
// Save the effect of a command execution
void Command::saveEffect(const string& eff) {
    *effect = eff;
    Notify(this);
}

string Command::stringToLog() {
    return "Command executed: " + *commandText + " | Effect: " + *effect;
}



/*
    === CommandProcessor Class Implementation ===
    It processes and validates commands from console input
*/

// Default Constructor
CommandProcessor::CommandProcessor() {
    commands = new vector<Command*>();
}

// Copy Constructor
CommandProcessor::CommandProcessor(const CommandProcessor& other) {
    commands = new vector<Command*>();
    for (Command* cmd : *other.commands) {
        commands->push_back(new Command(*cmd));
    }
}

// Destructor
CommandProcessor::~CommandProcessor() {
    for(Command* cmd : *commands) {
        delete cmd;
    }
    delete commands;
}

// Assignment Operator
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        for (Command* cmd: * commands) {
            delete cmd;
        }
        commands->clear();

        for (Command* cmd : *other.commands) {
            commands->push_back(new Command(*cmd));
        }
    }
    return *this;
}

// String Insertion Operator
ostream& operator<<(ostream& os, const CommandProcessor& cp) {
    os << "CommandProcessor with " << cp.commands->size() << " command(s):";
    for (const Command* cmd : *cp.commands) {
        os << "\n - " << *cmd;
    }
    return os;
}

// Read command from console
string CommandProcessor::readCommand() {
    string input;
    cout << "\nEnter command: ";
    getline(cin, input);
    return input;
}

// Save command to command collection
void CommandProcessor::saveCommand(Command* cmd){
    if (cmd) {
        commands->push_back(cmd);
        Notify(this);
    }
}

// Get command
Command* CommandProcessor::getCommand() {
    string cmdText = readCommand();
    Command* cmd = new Command(cmdText);
    saveCommand(cmd);
    return cmd;
}

// Validate command based on game state
bool CommandProcessor::validate(const string& command, GameEngine* engine)  {
    if (!engine) {
        return false;
    }
    string currentState = engine->getCurrentStateName();

    // Parse command by extracting the first word
    stringstream ss(command);
    string cmd;
    ss >> cmd;

    if (currentState == "start") {
        return (cmd == "loadmap");
    } else if (currentState == "map loaded") {
        return (cmd == "loadmap" || cmd == "validatemap");
    } else if (currentState == "map validated") {
        return (cmd == "addplayer");
    } else if (currentState == "players added") {
        return (cmd == "addplayer" || cmd == "assigncountries");
    } else if (currentState == "assign reinforcement") {
        return (cmd == "issueorder");
    } else if (currentState == "issue orders") {
        return (cmd == "issueorder" || cmd == "endissueorders");
    } else if (currentState == "execute orders") {
        return (cmd == "execorder" || cmd == "endexecorders" || cmd == "win");
    } else if (currentState == "win") {
        return (cmd == "play" || cmd == "end");
    }
    return false;
}

// Get command list
const vector<Command*>* CommandProcessor::getCommandList() const {
    return commands; 
}

string CommandProcessor::stringToLog() {
    if (commands->empty()) {
        return "CommandProcessor: no commands yet.";
    }
    Command* lastCommand = commands->back();
    return "Command saved: " + lastCommand->getCommandText();
}


/*
    === FileLineReader Class Implementation
    Reads lines from a text file
*/ 

// Default Constructor
FileLineReader::FileLineReader() {
    filename = new string("");
    fileStream = nullptr;
    lastLineRead = new string("");
}

// Constructor with parameter
FileLineReader::FileLineReader(const string& file) {
    filename = new string(file);
    fileStream = new ifstream(*filename);
    lastLineRead = new string("");
    if (!fileStream->is_open()) {
        cerr << "Error: Could not open the wanted file " << *filename << endl;
        delete fileStream;
        fileStream = nullptr;
    }
}

// Copy Constructor
FileLineReader::FileLineReader(const FileLineReader& other) {
    filename = new string(*other.filename);
    lastLineRead = new string(*other.lastLineRead);
    if (!filename->empty()) {
        fileStream = new ifstream(*filename);
    } else {
        fileStream = nullptr;
    }
}

// Destructor
FileLineReader::~FileLineReader() {
    if (fileStream) {
        if (fileStream->is_open()) {
            fileStream->close();
        }
        delete fileStream;
    }
    delete filename;
    delete lastLineRead;
}

// Assignment Operator
FileLineReader& FileLineReader::operator=(const FileLineReader& other) {
    if (this != &other) {
        if (fileStream) {
            if (fileStream->is_open()) {
                fileStream->close();
            }
            delete fileStream;
        }
        delete filename;
        delete lastLineRead;

        filename = new string(*other.filename);
        lastLineRead = new string(*other.lastLineRead);
        if(!filename->empty()) {
            fileStream = new ifstream(*filename);
        } else {
            fileStream = nullptr;
        }
    }
    return *this;
}

// Stream Insertion Operator
ostream& operator<<(ostream& os, const FileLineReader& reader) {
    os << "FileLineReader for file: " << *reader.filename;
    return os;
}

// Read line from file
string FileLineReader::readLineFromFile() {
    string line;
    if (fileStream && fileStream->is_open() && getline(*fileStream, line)) {
        Notify(this);
        return line;
    }
    return "";
}

string FileLineReader::stringToLog() {
    if (lastLineRead->empty()) {
        return "FileLineReader: reached end of file or read an empty line from " + *filename;
    }
    return "FileLineReader: read line '" + *lastLineRead + "' from file " + *filename;
}



/*
    === FileCommandProcessorAdapter Class Implementation
    It adapts CommandProcessor to read from files using FileLineReader class
*/

// Default Constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter() : CommandProcessor(), flr(nullptr) {
}

// Constructor with parameter
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string& filename) : CommandProcessor() {
    flr = new FileLineReader(filename);
}

// Copy Constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other) : CommandProcessor(other) {
    if (other.flr) {
        flr = new FileLineReader(*other.flr);
    } else {
        flr = nullptr;
    }
}

// Destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
}

// Assignment Operator
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& other) {
    if (this != &other) {
        CommandProcessor::operator=(other);
        delete flr;
        
        if (other.flr) {
            flr = new FileLineReader(*other.flr);
        } else {
            flr = nullptr;
        }
    }
    return *this;
}

// Stream Insertion Operator
ostream& operator<<(ostream& os, const FileCommandProcessorAdapter& adapter) {
    os << "FileCommandProcessorAdapter";
    if (adapter.flr) {
        os << " using " << *adapter.flr;
    }
    return os;
}

// Override readCommand method to read from a file instead of the console
string FileCommandProcessorAdapter::readCommand() {
    if (flr) {
        string line = flr->readLineFromFile();
        if(!line.empty()) {
            cout << "Read from file: " << line << endl;
            return line;
        }
    }
    return "";
}

string FileCommandProcessorAdapter::stringToLog() {
    if (!flr) {
        return "FileCommandProcessorAdapter: no file reader attached.";
    }

    return "FileCommandProcessorAdapter: reading commands from file.";
}
