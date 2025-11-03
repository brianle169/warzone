
#ifndef CommandProcessing_H
#define CommandProcessing_H

#include <iostream>
#include <memory>
#include <list>
#include "LoggingObserver.h"

using namespace std;

class Command;
class CommandProcessor;

typedef shared_ptr <Command> SpCommand;
typedef shared_ptr <CommandProcessor> SpCommandProcessor;
class CommandProcessor : public Subject, ILoggable {
private:
    list<SpCommand> spCommandList;
    shared_ptr<string> readCommand();
    SpCommand saveCommand(shared_ptr<string> cmdStr);
    
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other);
    ~CommandProcessor();
    CommandProcessor& operator= (const CommandProcessor& other);
    friend ostream& operator<< (ostream& os, const CommandProcessor& commandProcessor);
    SpCommand getCommand();
    void validate();
};


class Command : public Subject, ILoggable {
    private:
        shared_ptr<string> commandStr;
        shared_ptr<string> effectStr;

    public: 
        Command();
        Command(const string& commandStr);
        Command(const Command& other);
        ~Command();   
        Command& operator= (const Command& other);
        friend ostream& operator<< (ostream& os, const Command& command);
        void saveEffect(); 
};

// class FileCommandProcessorAdapter : public CommandProcessor {
//     private:
//         FileLineReader flr;
//         void readCommand();
//     public:
//         FileCommandProcessorAdapter(/*args*/);
//         ~FileCommandProcessorAdapter();

// };

// class FileLineReader {
//     private:
//         string readLineFromFile();
//     public:
//         FileLineReader(/*args*/);
//         ~FileLineReader();

// };

#endif
