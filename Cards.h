#ifndef Cards_H
#define Cards_H

// Defines card type
enum class CardType {Bomb, Reinforcement, Blockade, Airlift, Diplomacy};


class Card {
    public: 
        virtual void play() = 0; // Virtual enables polymorphism, = 0 makes it abstract 
};


class BombCard : Card {
    public: 
        virtual void play(); // virtual technincally implicit 

};


class Deck {
    public: 
        void draw();
        // Draw random card
        // Place it in the hand 
};

class Hand {
    public: 
        // Collection of cards

};
#endif
