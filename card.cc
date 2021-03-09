#include "card.h"
using namespace std;

Card::Card(int val, string suit) : val{val}, suit{suit}, isJokerAssigned{false} {
    if (val == 11) {
		sym = "J";
	} else if (val == 12) {
		sym = "Q";
	} else if (val == 13) {
		sym = "K";
	} else if (val == 1) {
		sym = "A";
	} else {
		sym = to_string(val);
	}
}

Card::~Card() {}

int Card::getVal() {
    return val;
}

std::string Card::getSuit() {
    return suit;
}

std::string Card::getSym() {
    return sym;
}

void Card::setVal(int setTo) {
    val = setTo;
}

void Card::setSuit(std::string setTo) {
    suit = setTo;
}

void Card::setSym(std::string setTo) {
    sym = setTo;
}

bool Card::isAssignedJokerCard() {
	if (isJokerAssigned) {
		return true;
	} 
	return false;
}

void Card::assignJoker() {
    isJokerAssigned = true;
}

void Card::unassignJoker() {
    isJokerAssigned = false;
	sym = "J";
}
