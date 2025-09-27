#include "Cards.h"

int main(){
    // Testing testing for funsies funsies 
    BombCard card;
    Card* c = &card;
    card.play();
    c->play();
    Card& c2 = card;
    c2.play();
    cout << card << endl;

    ReinforcementCard r;
    c = &r;
    c->play();

}
