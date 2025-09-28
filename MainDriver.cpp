#include <iostream>
#include "Cards.h"

using namespace std;

int main() {
    cout << "--- Starting Test ---" << endl;

    // Create deck and hand
    Deck deck;
    Hand hand;

    // Create some cards
    BombCard* bomb = new BombCard();
    ReinforcementCard* reinf = new ReinforcementCard();
    BlockadeCard* block = new BlockadeCard();
    AirliftCard* air = new AirliftCard();
    DiplomacyCard* dip = new DiplomacyCard();

    cout << "\n--- Adding cards to deck ---" << endl;
    deck.add(bomb);
    deck.add(reinf);
    deck.add(block);
    deck.add(air);
    deck.add(dip);

    cout << deck << endl;

    cout << "\n--- Drawing cards into hand ---" << endl;
    deck.draw(hand);
    deck.draw(hand);
    deck.draw(hand);

    cout << deck << endl;
    cout << hand << endl;

    cout << "\n--- Playing cards from hand ---" << endl;
    // Play one of the cards in hand
    bomb->play(deck, hand);   // should move bomb from hand back to deck
    reinf->play(deck, hand);  // same for reinforcement

    cout << deck << endl;
    cout << hand << endl;

    cout << "\n--- Attempting invalid play ---" << endl;
    // Play a card that's not in the hand (already moved back)
    bomb->play(deck, hand);

    cout << "\n--- Test Done ---" << endl;

    // cleanup
    delete bomb;
    delete reinf;
    delete block;
    delete air;
    delete dip;

    return 0;
}
