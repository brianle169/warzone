#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

#ifndef CommandProcessor_H
#define CommandProcessor_H

class GameEngine; 

// Represents a game command with its text and execution effect
class Command {
    private:
        string* commandText; // The command entered by the user
        string* effect;      // The result of the command execution
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

// The following class processes commands from console and validates them against game states
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
        // Reads command from input source
        virtual string readCommand();
        // Saves command to history
        void saveCommand(Command* cmd);
};


// The following class reads commands line-by-line from a text file
class FileLineReader {
    private: 
        string* filename;       // Name of the file to read
        ifstream* fileStream;   // File input stream
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

// The follwing is the Adapter class that reads commands from file instead of console
class FileCommandProcessorAdapter : public CommandProcessor {
    private:
        FileLineReader* flr; // FileLineReader poointer obj for file operations
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