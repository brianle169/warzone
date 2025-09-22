#include <iostream>
#include <string>
#include <vector>
#include "Player.h"

using namespace std;

int Player::ID = 0; // Initialize static member variable

Player::Player() {
	this->pname = new string("");
}

Player::Player(string* name) {
	this->pname = name; // This is a pointer assignment.
	cout << "Player " << *this->pname << " created." << std::endl;
}

ostream& operator<< (ostream& out, const Player& player) {
	out << "Player Name: " << *(player.pname);
	return out; // out = output stream
}

istream& operator>> (istream& in, const Player& player) {
	cout << "Enter new name for player " << ++player.ID << ": ";
	in >> *(player.pname);
	return in; // in = input stream
}

int main() {
	Player p1, p2;
	cout << "Program started, please enter player names." << endl;
	cin >> p1;
	cin >> p2;
	cout << p1 << endl;
	cout << p2 << endl;
	return 0;
}
