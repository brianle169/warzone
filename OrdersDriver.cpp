#include "Orders.h"
#include <iostream>

// Simple test classes for demonstration purposes
class Player {
public:
    std::string name;
    Player(const std::string& n) : name(n) {}
};

class Territory {
public:
    std::string name;
    Player* owner;
    int armies;
    
    Territory(const std::string& n) : name(n), owner(nullptr), armies(0) {}
};

int main() {
    


};
