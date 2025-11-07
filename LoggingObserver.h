#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H

#include <string>
#include <iostream>
#include <list>
#include <memory>

// ================= ILoggable =================
// Interface for any class whose state/actions should be logged
class ILoggable {
public:
    ILoggable();
    virtual ~ILoggable();

    // Copy constructor
    ILoggable(const ILoggable& other);

    // Assignment operator
    ILoggable& operator=(const ILoggable& other);

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, ILoggable& loggable);

    // Must be implemented by derived classes
    virtual std::string stringToLog() = 0;
};

// ================= Observer =================
// Abstract class that wants to be notified by a Subject
class Observer {
public:
    Observer();
    virtual ~Observer();

    // Copy constructor
    Observer(const Observer& other);

    // Assignment operator
    Observer& operator=(const Observer& other);

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, Observer& obs);

    // Must be implemented by derived classes
    virtual void Update(ILoggable* loggable) = 0;
};

// ================= Subject =================
// Maintains a list of observers and notifies them on events
class Subject {
private:
    std::list<std::shared_ptr<Observer>> observers;

public:
    Subject();
    virtual ~Subject();

    // Copy constructor
    Subject(const Subject& other);

    // Assignment operator
    Subject& operator=(const Subject& other);

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, Subject& subj);

    // Observer management
    virtual void Attach(std::shared_ptr<Observer> obs);
    virtual void Detach(std::shared_ptr<Observer> obs);
    virtual void Notify(ILoggable* loggable);
};

// ================= LogObserver =================
// Concrete Observer that writes log messages to a file
class LogObserver : public Observer {
public:
    LogObserver();
    ~LogObserver() override;

    // Copy constructor
    LogObserver(const LogObserver& other);

    // Assignment operator
    LogObserver& operator=(const LogObserver& other);

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, LogObserver& lo);

    void Update(ILoggable* loggable) override;
};

void testLoggingObserver();

#endif // LOGGINGOBSERVER_H
