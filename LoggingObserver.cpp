#include <iostream>
#include <string>
#include <fstream>
#include "LoggingObserver.h"

// ============= ILoggable class (interface) ===============

ILoggable::ILoggable(){}
ILoggable::~ILoggable(){}

// ============= Observer class (abstract) ===============

Observer::Observer(){}
Observer::~Observer(){}

// ============= Subject class ===============

Subject::Subject(){
    observers = new std::list<Observer*>;
}
Subject::~Subject(){
    delete observers;
}

void Subject::Attach(Observer* obs){
    observers->push_back(obs);
}

void Subject::Detach(Observer* obs){
    observers->remove(obs);
}

void Subject::Notify(ILoggable* loggable){
    for (Observer* obs : *observers){
            obs->Update(loggable);
        }
}
    
// ============= LogObserver class ===============

LogObserver::LogObserver(){}
LogObserver::~LogObserver(){}

void LogObserver::Update(ILoggable* loggable){

    std::ofstream file("gamelog.txt", std::ios::app);

    if (file.is_open()){
        file << loggable->stringToLog() << std::endl;
        file.close();
    }
}

int main(){
    
}