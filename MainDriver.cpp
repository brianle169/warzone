#include <iostream>
#include "Cards.h"
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "LoggingObserver.h"
using namespace std;

int main() {
    // Testing Drivers
    cout << endl << endl;
    cout << "==================== Testing Cards Driver ====================";
    cout << endl;
    testCards();
    cout << endl << endl;
    cout << "==================== Testing Map Driver ====================";
    cout << endl;
    testLoadMaps();
    cout << endl << endl;
    cout << "==================== Testing Players Driver ====================";
    cout << endl;
    testPlayers();
    cout << endl << endl;
    cout << "==================== Testing Orders Driver ====================";
    cout << endl;
    testOrdersList();
    cout << endl << endl;
    cout << "==================== Testing Game Engine Driver ====================";
    cout << endl;
    testGameStates();
    cout << endl << endl;
    cout << "==================== Testing Logging Observer Driver ====================";
    cout << endl;
    testLoggingObserver();

    return 0;
}
