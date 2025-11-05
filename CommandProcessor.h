#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

#ifndef CommandProcessor_H
#define CommandProcessor_H

class GameEngine; 

class Command {
    private:
        string* commandText;
        string* effect;
    public:
        // Default Constructor
        Command();
        // Constructor with parameter
        Command(const string& cmd);
        // Copy Constructor
        Command(const Command& other);
        // Destructor
        ~Command();
        // Assignment Operator
        Command& operator=(const Command& other);
        // Stream Insertion Operator
        friend ostream& operator<<(ostream& os, const Command& cmd);
        // Getters
        string getCommandText() const;
        string getEffect() const;
        // Save the effect of a command execution
        void saveEffect(const string& eff);
};

class CommandProcessor {
    private:
        // Collection of Command objects
        vector<Command*>* commands;
    public:
        // Default Constructor 
        CommandProcessor();
        // Copy Constructor
        CommandProcessor(const CommandProcessor& other);
        // Destructor
        virtual ~CommandProcessor();
        // Assignment Operator
        CommandProcessor& operator=(const CommandProcessor& other);
        // Stream Insertion Operator
        friend ostream& operator<<(ostream& os, const CommandProcessor& cp);
        // Get command from user
        Command* getCommand();
        // Validate command
        bool validate(const string& command, GameEngine* engine);
        // Get the list of all the commands
        const vector<Command*>* getCommandList() const;
    protected:
        virtual string readCommand();
        void saveCommand(Command* cmd);
};

class FileLineReader {
    private: 
        string* filename;
        ifstream* fileStream;
    public:
        // Default Constructor
        FileLineReader();
        // Constructor with parameter
        FileLineReader(const string& file);
        // Copy Constructor
        FileLineReader(const FileLineReader& other);
        // Destructor
        ~FileLineReader();
        // Assignment Operator
        FileLineReader& operator=(const FileLineReader& other);
        // Stream Insertion Operator
        friend ostream& operator<<(ostream& os, const FileLineReader& reader);
        // Read next line from file
        string readLineFromFile();
};

class FileCommandProcessorAdapter : public CommandProcessor {
    private:
        FileLineReader* flr;
    public:
        // Default Constructor
        FileCommandProcessorAdapter();
        // Constructor with parameter
        FileCommandProcessorAdapter(const string& filename);
        // Copy Constructor
        FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
        // Destructor
        ~FileCommandProcessorAdapter() override;
        // Assignment Operator
        FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);
        // Stream Insertion Operator
        friend ostream& operator<<(ostream& os, const FileCommandProcessorAdapter& adapter);
    protected:
        // We override readCommand method but this time to read from file instead of from the console
        string readCommand() override;
};
#endif