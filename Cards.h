#ifndef Cards_H
#define Cards_H
#include <memory>
#include <iostream>
#include <vector>

using namespace std;
class Order;
class Card;
class Deck; 
class Hand; 
typedef shared_ptr < Card > SpCard; // Type alias 
typedef shared_ptr < Hand > SpHand; // Type alias 
typedef shared_ptr < Order > SpOrder; // Type alias 


// After assignment 1, include Player and Order h files 
// PROFESSOR GAVE THE OK
// Temp Player and Order classes: 

// Represents an order in the game.
class Order {
    // Print the order
    friend ostream& operator<< (ostream& os, const Order& order);
    public: 
        Order();
    
};


// Represents a player in the game. 
// Stores a hand of cards and a list of orders.
class Player {
    SpHand hand;
    vector<SpOrder> orders; // should the vectors themselves be pointers?
    public: 
    // Create a player with the given hand
    Player(SpHand hand);
    // Add a new order to the player's list
    void addOrder(SpOrder order);

    // Print the player's orders
    friend ostream& operator<< (ostream& os, const Player& player);
    
};


// Abstract Card class
class Card {
    public: 
        // Virtual enables polymorphism, = 0 makes it abstract 
        virtual ~Card(); // Destructor 
        Card& operator=(const Card&);
        virtual void play(Deck& deck, Hand& hand, Player& player); 
        virtual void print(ostream& os) const = 0; // For overloading stream operators with polymorphism 
};

// Stream insertion operator 
ostream& operator<<(ostream& os, const Card& c);


// A child of Card that can be played by the player
class BombCard : public Card {
    public: 
        // Constructors 
        BombCard();
        BombCard(const BombCard& other);
        ~BombCard();
        // Assignment operator
        BombCard& operator=(const BombCard& other);
        // Prints the BombCard name
        virtual void print(ostream& os) const override;
};


// A child of Card that can be played by the player
class ReinforcementCard : public Card {
    public: 
        // Constructors 
        ReinforcementCard();
        ReinforcementCard(const ReinforcementCard& other);
        ~ReinforcementCard();
        // Assignment operator
        ReinforcementCard& operator=(const ReinforcementCard& other);
        // Prints the ReinforcementCard name
        virtual void print(ostream& os) const override;
};


// A child of Card that can be played by the player
class BlockadeCard : public Card {
    public: 
        // Constructors 
        BlockadeCard();
        BlockadeCard(const BlockadeCard& other);
        ~BlockadeCard();
        // Assignment operator
        BlockadeCard& operator=(const BlockadeCard& other);
        // Prints the BlockadeCard name
        virtual void print(ostream& os) const override;
};


// A child of Card that can be played by the player
class AirliftCard : public Card {
    public: 
        // Constructors 
        AirliftCard();
        AirliftCard(const AirliftCard& other);
        ~AirliftCard();
        // Assignment operator
        AirliftCard& operator=(const AirliftCard& other);
        // Prints the AirliftCard name
        virtual void print(ostream& os) const override;
};


// A child of Card that can be played by the player
class DiplomacyCard : public Card {
    public: 
        // Constructors 
        DiplomacyCard();
        DiplomacyCard(const DiplomacyCard& other);
        ~DiplomacyCard();
        // Assignment operator
        DiplomacyCard& operator=(const DiplomacyCard& other);
        virtual void print(ostream& os) const override;
};

// Represents a deck of cards in the game.
// Stores shared pointers to Card objects.
class Deck {
    private:
        vector< SpCard > spCards; // all cards in the deck
    public: 
        // Constructors 
        Deck();
        Deck(const Deck& other);
        ~Deck();
        // Assignment operator
        Deck& operator=(const Deck& other);
        // Stream insertion operator 
        friend ostream& operator<< (ostream& out, const Deck& deck);

        // Removes card pointer from the deck and returns the pointer to that card 
        SpCard draw();
        // Adds a card pointer to the deck 
        void add(SpCard card);
};

// Represents a player's hand of cards.
// Stores shared pointers to Card objects.
class Hand {
    private:
        vector< SpCard > spCards; // all cards in the deck
    public: 
        // Constructors 
        Hand();
        Hand(const Hand& other);
        ~Hand();
        //Assignment operator 
        Hand& operator=(const Hand& other);
        // Stream insertion operator 
        friend ostream& operator<< (ostream& out, const Hand& hand);
        // Adds a card pointer to the hand 
        void add(SpCard card);
        // Removes the card pointer from the hand and returns the pointer 
        SpCard remove(int index);
};

#endif
