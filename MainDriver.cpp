#include <iostream>
#include "Cards.h"
using namespace std;

int main() {
    cout << "=== Creating cards on the stack ===" << endl;
    BombCard bomb;
    ReinforcementCard reinforce;
    BlockadeCard blockade;
    AirliftCard airlift;
    DiplomacyCard diplomacy;

    cout << "\n=== Printing individual cards ===" << endl;
    cout << bomb << endl;
    cout << reinforce << endl;
    cout << blockade << endl;
    cout << airlift << endl;
    cout << diplomacy << endl;

    cout << "\n=== Playing individual cards ===" << endl;
    bomb.play(); cout << endl;
    reinforce.play(); cout << endl;
    blockade.play(); cout << endl;
    airlift.play(); cout << endl;
    diplomacy.play(); cout << endl;

    cout << "\n=== Creating deck and hand ===" << endl;
    Deck deck;
    Hand hand;

    cout << "\n=== Adding cards to deck ===" << endl;
    deck.add(make_shared<BombCard>(bomb));
    deck.add(make_shared<ReinforcementCard>(reinforce));
    deck.add(make_shared<BlockadeCard>(blockade));
    deck.add(make_shared<AirliftCard>(airlift));
    deck.add(make_shared<DiplomacyCard>(diplomacy));

    cout << "\nDeck contents: " << deck << endl;

    cout << "\n=== Drawing cards into hand ===" << endl;
    drawCard(deck, hand);
    drawCard(deck, hand);
    drawCard(deck, hand);

    cout << "\nDeck after draws: " << deck << endl;
    cout << "Hand after draws: " << hand << endl;

    cout << "\n=== Playing a card from hand ===" << endl;
    // Instead of asking for user input, just remove index 0 for testing
    SpCard played = hand.remove(0);
    if (played) {
        played->play();
        cout << endl;
        deck.add(played);
    }

    cout << "\nDeck after playing: " << deck << endl;
    cout << "Hand after playing: " << hand << endl;

    cout << "\n=== Copy construction and assignment tests ===" << endl;
    Deck deck2 = deck;   // copy constructor
    Hand hand2 = hand;   // copy constructor

    deck2 = deck;        // assignment operator
    hand2 = hand;        // assignment operator

    cout << "\nFinal Deck2: " << deck2 << endl;
    cout << "Final Hand2: " << hand2 << endl;

    cout << "\n=== Destructors will fire at program end ===" << endl;

    return 0;
}
