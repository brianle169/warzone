#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"

void testOrderExecution() {
    // Initializing 2 players
    Player* player1 = new Player("Anas");
    Player* player2 = new Player("West");

    // Initializing 4 territories
    Territory* territory1 = new Territory("Canada", nullptr, 100);
    Territory* territory2 = new Territory("USA", nullptr, 50);
    Territory* territory3 = new Territory("Japan", nullptr, 300);
    Territory* territory4 = new Territory("Germany", nullptr, 500);

    // Making some territories adjacent to eachother
    territory4->addEdge(territory1);
    territory1->addEdge(territory4);
    territory3->addEdge(territory2);
    territory2->addEdge(territory3);
    territory1->addEdge(territory2);
    territory2->addEdge(territory1);

    // Initializing cards for each players
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

    // Adding cards to each player's hand
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

    // Setting a player to territories
    territory1->setPlayer(player1);
    territory2->setPlayer(player2);
    territory3->setPlayer(player1);
    territory4->setPlayer(player2);

    cout << "Testing OrdersList" << endl;

    OrdersList orders_list;

    // Adding orders to the orders list
    orders_list.addOrder( new Deploy(player1,territory1,10)); //working case
    orders_list.addOrder( new Deploy(player1,territory3,300)); //working
    orders_list.addOrder( new Advance(player1,150,territory1, territory3));//invalid not enough army
    orders_list.addOrder( new Advance(player1,150,territory3, territory2)); //conquer
    orders_list.addOrder( new Advance(player1,150,territory3, territory2)); //ownership transferred
    orders_list.addOrder( new Advance(player1,10,territory1, territory2)); //defense
    orders_list.addOrder( new Advance(player1,30,territory2, territory3)); //Invalid, since player does not own source territory
    orders_list.addOrder( new Deploy(player2,territory1,200)); //not working case
    orders_list.addOrder( new Blockade(player1, territory4)); //invalid,not his territory
    orders_list.addOrder( new Blockade(player1, territory1));// valid
    orders_list.addOrder( new Bomb(player1, territory1, territory1));//invalid,own territory
    orders_list.addOrder( new Bomb(player1, territory4, territory1));//valid
    orders_list.addOrder( new Airlift(player1,10,territory1,territory2));//invalid,not his territory
    orders_list.addOrder( new Airlift(player1,10,territory1,territory3));//valid
    orders_list.addOrder( new Negotiate(player1,player1));//invalid, same player
    orders_list.addOrder( new Negotiate(player1,player2));//valid
    orders_list.addOrder( new Advance(player1, 150, territory1, territory4));//not valid since negotiated (4)



    cout << orders_list << endl;

    cout << "\n2. Testing order validation..." << endl; //(1)
    for (int i = 0; i < static_cast<int>(orders_list.size()); ++i) {
        Order* order = orders_list.getOrder(i);
        if (order->validate()) {
            cout << order->getPlayer() << ": " << order->getName() << " STATE -> " << "Valid" << endl;
        } else {
            cout << order->getPlayer() << ": " << order->getName() << " STATE -> " << "Invalid" << endl;
        }
    }

    cout << "\nThe territory " << territory2->getName() << " is now owned by : " << *territory2->getPlayer(); //printing who owns the territory(2)

    cout << "\n3. Testing order execution..." << endl;
    for (int i = 0; i < static_cast<int>(orders_list.size()); ++i) {
        Order* order = orders_list.getOrder(i);
        if (order->validate()) {
            order->execute();
            cout << *order << endl;

        }
    }
    cout << "\nThe territory " << territory2->getName() << " is now owned by : " << *territory2->getPlayer(); //printing who owns the territory(2)

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

    // Cleanup
    delete player1; 
    delete player2;
    delete territory1;  
    delete territory2;  
    delete territory3; 
    delete territory4;
};