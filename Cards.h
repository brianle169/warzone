#ifndef Cards_H
#define Cards_H

#include <iostream>

using namespace std;

// Abstract Card class 
class Card {
    public: 
        // Virtual enables polymorphism, = 0 makes it abstract 
        virtual ~Card(){} // Destructor 
        Card& operator=(const Card&);
        virtual Card* clone() const = 0; // Const bcs copy should be read-only 
        virtual void play() = 0; 
        virtual void print(ostream& os) const = 0; // For overloading stream operators with polymorphism 
};

// Stream insertion operator 
ostream& operator<<(ostream& os, const Card& c);

// Do I need an istream operator overloading? 
// istream& operator>>(istream& os, const Card& c);


class BombCard : public Card {
    public: 
        BombCard();
        BombCard(const BombCard& other);
        ~BombCard();
        BombCard& operator=(const BombCard& other);
        virtual Card* clone() const override; // override avoids function signature mismatch 
        virtual void play(); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

class ReinforcementCard : public Card {
    public: 
        ReinforcementCard();
        ReinforcementCard(const ReinforcementCard& other);
        ~ReinforcementCard();
        ReinforcementCard& operator=(const ReinforcementCard& other);
        virtual Card* clone() const override; // override avoids function signature mismatch 
        virtual void play(); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

class BlockadeCard : public Card {
    public: 
        BlockadeCard();
        BlockadeCard(const BlockadeCard& other);
        ~BlockadeCard();
        BlockadeCard& operator=(const BlockadeCard& other);
        virtual Card* clone() const override; // override avoids function signature mismatch 
        virtual void play(); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

class AirliftCard : public Card {
    public: 
        AirliftCard();
        AirliftCard(const AirliftCard& other);
        ~AirliftCard();
        AirliftCard& operator=(const AirliftCard& other);
        virtual Card* clone() const override; // override avoids function signature mismatch 
        virtual void play(); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

class DiplomacyCard : public Card {
    public: 
        DiplomacyCard();
        DiplomacyCard(const DiplomacyCard& other);
        ~DiplomacyCard();
        DiplomacyCard& operator=(const DiplomacyCard& other);
        virtual Card* clone() const override; // override avoids function signature mismatch 
        virtual void play(); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

// class Deck {
//     public: 
//         void draw();
//         // Draw random card
//         // Place it in the hand 
// };

// class Hand {
//     public: 
//         // Collection of cards

// };
#endif
