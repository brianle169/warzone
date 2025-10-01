#ifndef Cards_H
#define Cards_H
#include <memory>
#include <iostream>
#include <vector>

using namespace std;
class Card;
class Deck; 
class Hand; 
typedef shared_ptr < Card > SpCard; // Type alias 

// Abstract Card class
class Card {
    public: 
        // Virtual enables polymorphism, = 0 makes it abstract 
        virtual ~Card(); // Destructor 
        Card& operator=(const Card&);
        virtual Card* clone() const = 0; // Const bcs copy should be read-only 
        virtual void play() = 0; 
        virtual void print(ostream& os) const = 0; // For overloading stream operators with polymorphism 
};

// Stream insertion operator 
ostream& operator<<(ostream& os, const Card& c);
istream& operator>>(istream& is, const Card& c);


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

class Deck {
    private:
        vector< SpCard > spCards; // all cards in the deck
    public: 
        Deck();
        Deck(const Deck& other);
        ~Deck();
        Deck& operator=(const Deck& other);
        friend ostream& operator<< (ostream& out, const Deck& deck);
        friend istream& operator>> (istream& in, const Deck& deck);


        SpCard draw();
        void add(SpCard card);
        // Draw random card
        // Place it in the hand (and remove it from deck)

};

class Hand {
    private:
        vector< SpCard > spCards; // all cards in the deck
    public: 
        Hand();
        Hand(const Hand& other);
        ~Hand();
        Hand& operator=(const Hand& other);
        friend ostream& operator<< (ostream& out, const Hand& hand);
        friend istream& operator>> (istream& in, const Hand& hand);
        void add(SpCard card);
        SpCard remove(int index);
};

// Helper functions 
void drawCard(Deck& deck, Hand& hand);
void playCard(Deck& deck, Hand& hand);

#endif
