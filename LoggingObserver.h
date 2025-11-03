#ifndef LoggingObserver_H
#define LoggingObserver_H

#include <string>
#include <iostream>
#include <list>

// Acts as an interface for any class whose state or actions should be recorded in the game log
class ILoggable {
    public:
        ILoggable();
        virtual ~ILoggable();
        virtual std::string stringToLog() = 0;
    };
    

// Represents an entity that wants to be notified when an event occurs in a Subject.
class Observer {
    public:
    Observer();
    virtual ~Observer();
    virtual void Update(ILoggable* loggable) = 0;

};

// Maintains a list of Observers and notifies them whenever an event occurs.
class Subject {
    private:
        std::list<Observer*> *observers;
    public:
        Subject();
        virtual ~Subject();
        virtual void Attach(Observer* obs);
        virtual void Detach(Observer* obs);
        virtual void Notify(ILoggable* loggable);
};

//  Implementation of an Observer that writes log messages to a file.
class LogObserver: public Observer {
    public:
    LogObserver();
    ~LogObserver() override;
    void Update(ILoggable* loggable) override;
};

#endif