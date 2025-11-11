#include "LoggingObserver.h"
#include <iostream>
#include <fstream>

// ================= ILoggable =================

ILoggable::ILoggable() {}
ILoggable::~ILoggable() {}

// Copy constructor
ILoggable::ILoggable(const ILoggable &) {}

// Assignment operator
ILoggable &ILoggable::operator=(const ILoggable &)
{
    return *this;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, ILoggable &loggable)
{
    os << loggable.stringToLog();
    return os;
}

// ================= Observer =================

Observer::Observer() {}
Observer::~Observer() {}

// Copy constructor
Observer::Observer(const Observer &) {}

// Assignment operator
Observer &Observer::operator=(const Observer &)
{
    return *this;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, Observer &obs)
{
    os << "Observer";
    return os;
}

// ================= Subject =================

Subject::Subject() {}
Subject::~Subject() {}

// Copy constructor
Subject::Subject(const Subject &other)
{
    // Deep copy of observers list
    observers.clear();
    for (auto &obs : other.observers)
    {
        observers.push_back(obs); // shared_ptr handles copying automatically
    }
}

// Assignment operator
Subject &Subject::operator=(const Subject &other)
{
    if (this != &other)
    {
        observers.clear();
        for (auto &obs : other.observers)
        {
            observers.push_back(obs); // shared_ptr copy
        }
    }
    return *this;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, Subject &subj)
{
    os << "Subject with " << subj.observers.size() << " observers";
    return os;
}

void Subject::Attach(std::shared_ptr<Observer> obs)
{
    observers.push_back(obs);
}

void Subject::Detach(std::shared_ptr<Observer> obs)
{
    observers.remove(obs); // might not find?
}

void Subject::Notify(ILoggable *loggable)
{
    for (std::shared_ptr<Observer> obs : observers)
    {
        obs->Update(loggable);
    }
}

// ================= LogObserver =================

LogObserver::LogObserver()
{
}
LogObserver::~LogObserver() {}

// Copy constructor
LogObserver::LogObserver(const LogObserver &other) : Observer(other)
{

    std::ofstream("gamelog.txt", std::ios::trunc).close();
}

// Assignment operator
LogObserver &LogObserver::operator=(const LogObserver &other)
{
    if (this != &other)
    {
        Observer::operator=(other);
    }
    return *this;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, LogObserver &lo)
{
    os << "LogObserver";
    return os;
}

void LogObserver::Update(ILoggable *loggable)
{
    std::ofstream file("gamelog.txt", std::ios::app);
    if (file.is_open())
    {
        file << loggable->stringToLog() << std::endl;
        file.close();
    }
}