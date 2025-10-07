#include <iostream>
#include "Cards.h"
#include "Player.h"

using namespace std;

void testCards(){
    // Initialize game objects
    Deck deck;
    Player player("Diana");

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
    player.getHand()->add(bc);
    player.getHand()->add(rc);
    player.getHand()->add(blc);
    player.getHand()->add(ac);
    player.getHand()->add(dc);


    // Make a copy of a card in the hand and try to add it to the hand
    cout << "\n========== Trying to add a copied card to the hand ============ \n";
    player.getHand()->add(cpy);
    
    cout << "\n" << *player.getHand() << "\n";

    // Play cards from the hand (creates orders, stores them in player's order list and adds them back to the deck)
    cout << "\n========== Playing cards from the Hand ============ \n";
    bc->play(deck, *player.getHand(), player);
    cout << "\n";
    rc->play(deck, *player.getHand(), player);
    cout << "\n";
    blc->play(deck, *player.getHand(), player);
    cout << "\n";
    ac->play(deck, *player.getHand(), player);
    cout << "\n";
    dc->play(deck, *player.getHand(), player);
    
    // Print hand contents 
    cout << "\n" << *player.getHand() << "\n";
    
    // Print deck contents 
    cout << "\n" << deck << "\n";

    cout << "\n========== Cleanup ============ \n";   
}