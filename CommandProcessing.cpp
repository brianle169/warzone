#include "CommandProcessing.h"
#include <iostream>
#include <list>

using namespace std;

CommandProcessor::CommandProcessor(){}
CommandProcessor::~CommandProcessor(){}

string CommandProcessor::readCommand(){
    string cmdStr;
    cout << "Enter command: " << endl;
    cin >> cmdStr; 
    return cmdStr;
}

Command* CommandProcessor::saveCommand(string cmdStr){
    Command* command = new Command(cmdStr);
    commandList.push_back(command);  
    return command;  
}

Command* CommandProcessor::getCommand(){
    string cmdStr = readCommand();
    Command* command = saveCommand(cmdStr);
    return command;
}

Command::Command(){}

Command::Command(const string& commandStr): commandStr(commandStr){}



