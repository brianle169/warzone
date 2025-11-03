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

Subject::Subject(){}
Subject::~Subject(){}

void Subject::Attach(std::shared_ptr<Observer> obs){
    observers.push_back(obs);
}

void Subject::Detach(std::shared_ptr<Observer> obs){
    observers.remove(obs); // might not find?
}

void Subject::Notify(ILoggable* loggable){
    for (std::shared_ptr<Observer> obs : observers){
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