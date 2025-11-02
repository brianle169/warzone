#include "CommandProcessor.h"

// === Command Class Implementation ===

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
}


// === CommandProcessor Class Implementation ===