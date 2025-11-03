#include "CommandProcessing.h"
#include <iostream>
#include <list>

using namespace std;

// Default constructor 
CommandProcessor::CommandProcessor(){}

// Destructor 
CommandProcessor::~CommandProcessor(){}

// Copy constructor 
CommandProcessor::CommandProcessor(const CommandProcessor& other) {
    for (auto& cmd : other.spCommandList){
        if (cmd){
            spCommandList.push_back(make_shared<Command>(*cmd));
        }
        else {
            spCommandList.push_back(nullptr);
        }
    }
    cout << "Command processor copied. " << endl;
}

// Assignment operator 
// Deep or shallow copy?
CommandProcessor& CommandProcessor::operator= (const CommandProcessor& other){
    // If not comparing to itself
	if (this != &other) {
		spCommandList.clear();
        for (const auto& cmd : other.spCommandList) {
            spCommandList.push_back(make_shared<Command>(*cmd)); 
        }
	}
	return *this;
}

// Stream insertion operator 
ostream& operator<< (ostream& os, const CommandProcessor& commandProcessor){
    os << "Current list of commands: \n";

    for (const auto& cmd : commandProcessor.spCommandList){
        if (cmd)
            os << *cmd;  // Command::operator<< already ends with endl
        else
            os << "null command" << endl;      
    }

    os << endl;
    return os;
}

shared_ptr<string> CommandProcessor::readCommand(){
    shared_ptr<string> cmdStr = make_shared<string>();
    cout << "Enter command: " << endl;
    cin >> *cmdStr; 
    return cmdStr;
}

SpCommand CommandProcessor::saveCommand(shared_ptr<string> cmdStr){
    SpCommand command = make_shared<Command>(*cmdStr);
    spCommandList.push_back(command);  
    
    return command;  
}

SpCommand CommandProcessor::getCommand(){
    shared_ptr<string> cmdStr = readCommand();
    SpCommand command = saveCommand(cmdStr);
    return command;
}

// Default constructor
Command::Command(){}

// Destructor
Command::~Command(){}

// Paramaterized constructor 
Command::Command(const string& commandStr): commandStr(new string(commandStr)){}

// Copy constructor 
Command::Command(const Command& other){
    if (other.commandStr) {
        this->commandStr = make_shared<string>(*(other.commandStr));
    }
    else {
        this->commandStr = nullptr;
    }

    if (other.effectStr){
        this->effectStr = make_shared<string>(*(other.effectStr));
    }
    else {
        this->effectStr = nullptr;
    }
}

// Assignment operator 
Command& Command::operator= (const Command& other){
    if (this != &other) {	
        // Does operator overloading require deep copies or shallow copies?
        // Deep copy
		this->commandStr = make_shared<string>(*(other.commandStr)); 
		this->effectStr = make_shared<string>(*(other.effectStr)); 
        // // Shallow copy
        // commandStr = other.commandStr;
        // effectStr = other.effectStr;
	}

	return *this;
}

// Stream insertion operator 
ostream& operator<< (ostream& os, const Command& command){
    os << "Command: " 
    << (command.commandStr ? *command.commandStr : "null")  
    << ", Effect: " 
    << (command.effectStr ? *command.effectStr : "null") 
    << endl;
    return os;
}





