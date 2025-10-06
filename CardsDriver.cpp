#include <iostream>
#include "Cards.h"

using namespace std;

void testCards(){
    // Initialize game objects 
    Deck deck;
    SpHand hand = SpHand(new Hand());
    Player player(hand);

    // Populate the deck 
    cout << "\n========== Populate the Deck ============ \n";
    deck.add(SpCard(new BombCard()));
    deck.add(SpCard(new ReinforcementCard()));
    deck.add(SpCard(new BlockadeCard()));
    deck.add(SpCard(new AirliftCard()));
    deck.add(SpCard(new DiplomacyCard()));

    // Print deck contents 
    cout << "\n" << deck << "\n";

    // Draw cards from the deck 
    cout << "\n========== Draw cards ============ \n";
    SpCard bc = deck.draw();
    SpCard rc = deck.draw();
    SpCard blc = deck.draw();
    SpCard ac = deck.draw();
    SpCard dc = deck.draw();

    // Create a test copy
    SpCard cpy = ac;
    
    // Print deck contents 
    cout << "\n" << deck << "\n";
    
    // Add the cards to the hand 
    cout << "\n========== Add cards to Hand ============ \n";
    hand->add(bc);
    hand->add(rc);
    hand->add(blc);
    hand->add(ac);
    hand->add(dc);


    // Make a copy of a card in the hand and try to add it to the hand
    cout << "\n========== Trying to add a copied card to the hand ============ \n";
    hand->add(cpy);
    
    cout << "\n" << *hand << "\n";

    // Play cards from the hand (creates orders, stores them in player's order list and adds them back to the deck)
    cout << "\n========== Playing cards from the Hand ============ \n";
    bc->play(deck, *hand, player);
    cout << "\n";
    rc->play(deck, *hand, player);
    cout << "\n";
    blc->play(deck, *hand, player);
    cout << "\n";
    ac->play(deck, *hand, player);
    cout << "\n";
    dc->play(deck, *hand, player);


    // Print player's order list 
    cout << "\n" << player << "\n";
    
    // Print hand contents 
    cout << "\n" << *hand << "\n";
    
    // Print deck contents 
    cout << "\n" << deck << "\n";

    cout << "\n========== Cleanup ============ \n";   
}
