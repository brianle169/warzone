#ifndef Cards_H
#define Cards_H

#include <iostream>

using namespace std;

// Abstract Card class 
class Card {
    public: 
        // Virtual enables polymorphism, = 0 makes it abstract 
        virtual ~Card(){} // Destructor 
        // virtual Card* copy() const = 0; // Const bcs copy should be read-only 
        virtual void play() = 0; 
        virtual void print(ostream& os) const = 0; // For overloading stream operators with polymorphism 
        // Still need assignment operator 
};

// Stream insertion operator 
ostream& operator<<(ostream& os, const Card& c);

// Do I need an istream operator overloading? 
// istream& operator>>(istream& os, const Card& c);


class BombCard : public Card {
    public: 
        virtual void play(); // virtual technincally implicit 
        virtual void print(ostream& os) const override;

};

class ReinforcementCard : public Card {
    public: 
        virtual void play(); 
        virtual void print(ostream& os) const override;
};

class BlockadeCard : public Card {
    public: 
        virtual void play(); 
        virtual void print(ostream& os) const override;
};

class AirliftCard : public Card {
    public: 
        virtual void play(); 
        virtual void print(ostream& os) const override;
};

class DiplomacyCard : public Card {
    public: 
        virtual void play(); 
        virtual void print(ostream& os) const override;
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
