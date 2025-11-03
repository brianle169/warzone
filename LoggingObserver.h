#ifndef LoggingObserver_H
#define LoggingObserver_H

#include <string>
#include <iostream>
#include <list>

class ILoggable {
    public:
        ILoggable();
        virtual ~ILoggable();
        virtual std::string stringToLog() = 0;
    };
    
    class Observer {
        public:
        Observer();
        virtual ~Observer();
        virtual void Update(ILoggable* loggable) = 0;
    
    };

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

class LogObserver: public Observer {
    public:
    LogObserver();
    ~LogObserver() override;
    void Update(ILoggable* loggable) override;
};

#endif