#include "CommandProcessing.h"
#include <iostream>

using namespace std;

int main() {

    CommandProcessor commandProcessor;

    cout << commandProcessor;
    
    commandProcessor.getCommand();
    commandProcessor.getCommand();
    
    cout << commandProcessor;


}