#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"

void testOrdersList() {
    Player* player1 = new Player("Anas");
    Player* player2 = new Player("West");

    Territory* territory1 = new Territory("Canada", nullptr, 100);
    Territory* territory2 = new Territory("USA", nullptr, 50);
    Territory* territory3 = new Territory("Japan", nullptr, 300);
    Territory* territory4 = new Territory("Germany", nullptr, 500);
    territory4->addEdge(territory1);
    territory1->addEdge(territory4);
    territory3->addEdge(territory2);
    territory2->addEdge(territory3);
    territory1->addEdge(territory2);
    territory2->addEdge(territory1);

    SpCard bc1 = SpCard(new BombCard());
    SpCard rc1 = SpCard(new ReinforcementCard());
    SpCard blc1 = SpCard(new BlockadeCard());
    SpCard ac1 = SpCard(new AirliftCard());
    SpCard dc1 = SpCard(new DiplomacyCard());

    SpCard bc2 = SpCard(new BombCard());
    SpCard rc2 = SpCard(new ReinforcementCard());
    SpCard blc2 = SpCard(new BlockadeCard());
    SpCard ac2 = SpCard(new AirliftCard());
    SpCard dc2 = SpCard(new DiplomacyCard());

    player1->getHand()->add(bc1);
    player1->getHand()->add(rc1);
    player1->getHand()->add(blc1);
    player1->getHand()->add(ac1);
    player1->getHand()->add(dc1);

    player2->getHand()->add(bc2);
    player2->getHand()->add(rc2);
    player2->getHand()->add(blc2);
    player2->getHand()->add(ac2);
    player2->getHand()->add(dc2);

    territory1->setPlayer(player1);
    territory2->setPlayer(player2);
    territory3->setPlayer(player1);
    territory4->setPlayer(player2);

    cout << "Testing OrdersList" << endl;

    OrdersList orders_list;

    orders_list.addOrder( new Deploy(player1,territory1,10)); //working case
    orders_list.addOrder( new Deploy(player1,territory3,300)); //working
    orders_list.addOrder( new Advance(player1,150,territory1, territory3));//invalid not enough army
    orders_list.addOrder( new Advance(player1,150,territory3, territory2)); //conquer
    orders_list.addOrder( new Advance(player1,10,territory1, territory2)); //defense
    orders_list.addOrder( new Deploy(player2,territory1,200)); //not working case
    orders_list.addOrder( new Blockade(player1, territory4)); //invalid,not his territory
    orders_list.addOrder( new Blockade(player1, territory1));// valid
    orders_list.addOrder( new Bomb(player1, territory1, territory1));//invalid,own territory
    orders_list.addOrder( new Bomb(player1, territory4, territory1));//valid
    orders_list.addOrder( new Airlift(player1,10,territory1,territory2));//invalid,not his territory
    orders_list.addOrder( new Airlift(player1,10,territory1,territory3));//valid
    orders_list.addOrder( new Negotiate(player1,player1));//invalid, same player
    orders_list.addOrder( new Negotiate(player1,player2));//valid



    cout << orders_list << endl;

    cout << "\n2. Testing order validation..." << endl;
    for (int i = 0; i < static_cast<int>(orders_list.size()); ++i) {
        Order* order = orders_list.getOrder(i);
        if (order->validate()) {
            cout << order->getPlayer() << ": " << order->getName() << " STATE -> " << "Valid" << endl;
        } else {
            cout << order->getPlayer() << ": " << order->getName() << " STATE -> " << "Invalid" << endl;
        }
    }

    cout << "\n3. Testing order execution..." << endl;
    for (int i = 0; i < static_cast<int>(orders_list.size()); ++i) {
        Order* order = orders_list.getOrder(i);
        if (order->validate()) {
            order->execute();
            cout << *order << endl;

        }
    }

    cout << "\n4. Testing move operation..." << endl;
    orders_list.move(5,0);
    orders_list.move(13,1);
    cout << orders_list << endl;


    cout << "\n5. Testing remove operation..." << endl;
    orders_list.remove(0);
    orders_list.remove(0);
    orders_list.remove(0);
    orders_list.remove(0);
    orders_list.remove(9);
    cout << orders_list << endl;

    delete player1; 
    delete player2;
    delete territory1;  
    delete territory2;  
    delete territory3; 
    delete territory4;
};