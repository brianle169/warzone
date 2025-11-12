#include <iostream>

#include "Cards.h"
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "LoggingObserver.h"
#include "CommandProcessing.h"

using namespace std;

void game()
{
     GameEngine engine;
     engine.Attach(std::make_shared<LogObserver>());
     engine.startupPhase();
}

void test()
{
     int optionIndex = -1;
     while (true)
     {
          cout << "Enter the index of the driver you would like to test: \n"
               << "[0] testCommandProcessor() \n"
               << "[1] testStartupPhase() \n"
               << "[2] testMainGameLoop()\n"
               << "[3] testOrderExecution()\n"
               << "[4] testLoggingObserver()\n"
               << "[5] Quit\n"
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
          case 0:
               testCommandProcessor("all");
               cout << "testCommandProcessor() called\n";
               break;
          case 1:
               testStartupPhase();
               break;
          case 2:
               testMainGameLoop();
               break;
          case 3:
               testOrderExecution();
               break;
          case 4:
               testLoggingObserver();
               break;
          case 5:
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
