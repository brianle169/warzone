#ifndef Cards_H
#define Cards_H
#include <memory>
#include <iostream>
#include <vector>

using namespace std;
class Order;
class Card;
class Deck; 
class Player;
class Hand; 
typedef shared_ptr < Card > SpCard; // Type alias 
typedef shared_ptr < Hand > SpHand; // Type alias 
typedef shared_ptr < Order > SpOrder; // Type alias 


// After assignment 1, include Player and Order h files 
// Temp Player and Order classes: 

class Order {
    friend ostream& operator<< (ostream& os, const Order& order);
    public: 
        Order();
    
};


class Player {
    SpHand hand;
    vector<SpOrder> orders; // should the vectors themselves be pointers?
    public: 
    Player(SpHand hand);
    void addOrder(SpOrder order);
    
    friend ostream& operator<< (ostream& os, const Player& player);
    
};

// ostream& operator<<(ostream& os, const Player& player);

// Abstract Card class
class Card {
    public: 
        // Virtual enables polymorphism, = 0 makes it abstract 
        virtual ~Card(); // Destructor 
        Card& operator=(const Card&);
        virtual void play(Deck& deck, Hand& hand, Player& player) = 0; 
        virtual void print(ostream& os) const = 0; // For overloading stream operators with polymorphism 
};

// Stream insertion operator 
ostream& operator<<(ostream& os, const Card& c);

class BombCard : public Card {
    public: 
        BombCard();
        BombCard(const BombCard& other);
        ~BombCard();
        BombCard& operator=(const BombCard& other);
        virtual void play(Deck& deck, Hand& hand, Player& player); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

class ReinforcementCard : public Card {
    public: 
        ReinforcementCard();
        ReinforcementCard(const ReinforcementCard& other);
        ~ReinforcementCard();
        ReinforcementCard& operator=(const ReinforcementCard& other);
        virtual void play(Deck& deck, Hand& hand, Player& player); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

class BlockadeCard : public Card {
    public: 
        BlockadeCard();
        BlockadeCard(const BlockadeCard& other);
        ~BlockadeCard();
        BlockadeCard& operator=(const BlockadeCard& other);
        virtual void play(Deck& deck, Hand& hand, Player& player); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

class AirliftCard : public Card {
    public: 
        AirliftCard();
        AirliftCard(const AirliftCard& other);
        ~AirliftCard();
        AirliftCard& operator=(const AirliftCard& other);
        virtual void play(Deck& deck, Hand& hand, Player& player); // virtual technincally implicit but kept for clarity 
        virtual void print(ostream& os) const override;
};

class DiplomacyCard : public Card {
    public: 
        DiplomacyCard();
        DiplomacyCard(const DiplomacyCard& other);
        ~DiplomacyCard();
        DiplomacyCard& operator=(const DiplomacyCard& other);
        virtual void play(Deck& deck, Hand& hand, Player& player); // virtual technincally implicit but kept for clarity 
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

        SpCard draw();
        void add(SpCard card);
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
        void add(SpCard card);
        SpCard remove(int index);
};

#endif
