#include <iostream>

#include "Cards.h"
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "LoggingObserver.h"
#include "CommandProcessing.h"
#include "PlayerStrategies.h"

using namespace std;

void game()
{
     GameEngine engine;
     engine.Attach(std::make_shared<LogObserver>());
     engine.startupPhase();
}

void testTournament(); // Forward declaration

void test()
{
     int optionIndex = -1;
     while (true)
     {
          cout << "Enter the index of the driver you would like to test: \n"
               << "[1] testPlayerStrategies()\n"
               << "[2] testTournament()\n"
               << "[3] Quit\n"
               << ">> ";

          if (!(cin >> optionIndex))
          {
               cout << "Not an integer." << endl;
               cin.clear();
               cin.ignore(numeric_limits<streamsize>::max(), '\n');
               continue;
          }

          switch (optionIndex)
          {
          case 1:
               testPlayerStrategies();
               break;
          case 2:
               testTournament();
               break;
          case 3:
               cout << "Exiting test menu.\n";
               return;
          default:
               cout << "Invalid index\n";
               break;
          }
     }
}

int main(int argc, char *argv[])
{
     if (argc > 1)
     {
          string arg = argv[1];
          if (arg == "-test")
          {
               test();
          }
          else
          {
               cout << "Invalid arg." << endl;
          }
     }
     else
     {
          cout << "Playing game..." << endl;
          game();
     }
     return 0;
}
