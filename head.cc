#include "head.h"
using namespace std;

//My cards will be played/drawn from back to front 
//Top card being drawn from aGivenHeadPile to be played on is aGivenHeadPile.back();

Head::Head() {}

Head::~Head() {}

int Head::headPileSize() {
    return aGivenHeadPile.size();
}

std::string Head::topCard() {
    string retval = aGivenHeadPile.back()->getSym() + aGivenHeadPile.back()->getSuit();
    return retval;
	
}

int Head::getTopCardVal() {
    return aGivenHeadPile.back()->getVal();
}

std::shared_ptr<Card> Head::getTopCard() {
    return aGivenHeadPile.back();
}

string Head::getTopCardSuit() {
    return aGivenHeadPile.back()->getSuit();
}

void Head::addToHeadPile(std::shared_ptr<Card> card) {
    aGivenHeadPile.emplace_back(card);
}

void Head::cutHeads(std::shared_ptr<Player> player) {
    player->cutHeads(aGivenHeadPile);
}
