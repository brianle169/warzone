
#include <iostream>
#include <list>

#ifndef CommandProcessing_H
#define CommandProcessing_H

class CommandProcessor
{
private:
    std::list<Command*> commandList;
    void readCommand();
    
public:
    CommandProcessor(/* args */);
    ~CommandProcessor();
    void getCommand();
    void validate();
};


class Command {
    private:
        std::string commandStr;
        std::string effectStr;

    public: 
        Command(/* args */);
        ~Command();   
        void saveEffect(); 
};

class FileCommandProcessorAdapter : public CommandProcessor {
    private:
        FileLineReader flr;
        void readCommand();
    public:
        FileCommandProcessorAdapter(/*args*/);
        ~FileCommandProcessorAdapter();

};

class FileLineReader {
    private:
        void readLineFromFile();
    public:
        FileLineReader(/*args*/);
        ~FileLineReader();

};

#endif
