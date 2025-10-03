#include <iostream>
#include "Cards.h"

using namespace std;

void testCards(){
    Deck deck;
    SpHand hand = SpHand(new Hand());
    Player player(hand);

    cout << "\n========== Populate the Deck ============ \n";
    deck.add(SpCard(new BombCard()));
    deck.add(SpCard(new ReinforcementCard()));
    deck.add(SpCard(new BlockadeCard()));
    deck.add(SpCard(new AirliftCard()));
    deck.add(SpCard(new DiplomacyCard()));

    cout << "\n" << deck << "\n";

    cout << "\n========== Draw cards ============ \n";

    SpCard bc = deck.draw();
    SpCard rc = deck.draw();
    SpCard blc = deck.draw();
    SpCard ac = deck.draw();
    SpCard dc = deck.draw();

    cout << "\n" << deck << "\n";


    cout << "\n========== Add cards to Hand ============ \n";
    hand->add(bc);
    hand->add(rc);
    hand->add(blc);
    hand->add(ac);
    hand->add(dc);
    
    cout << "\n" << *hand << "\n";

    cout << "\n========== Playing cards from the Hand ============ \n";
    bc->play(deck, *hand, player);
    cout << "\n";
    rc->play(deck, *hand, player);
    cout << "\n";
    blc->play(deck, *hand, player);


    cout << "\n" << player << "\n";
    
    cout << "\n" << *hand << "\n";
    
    cout << "\n" << deck << "\n";

    cout << "\n========== Cleanup ============ \n";







    
}

// 4.2.4
// 4.2.9
// 4.2.10
// 4.3.2
// 4.5.1
// 4.5.3
int main(){
    testCards();    

}
