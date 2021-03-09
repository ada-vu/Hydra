#include "player.h"
using namespace std;

//My cards will be played/drawn from back to front 
//Top card being drawn from drawDeck to be played is drawDeck.back();

Player::Player() {}

Player::~Player() {}

void Player::addToReserve(std::shared_ptr<Card> card) {             
    reserveCard = card;
    drawDeck.pop_back();
}

void Player::swapWithReserve(std::shared_ptr<Card> card) {
    auto temp = make_shared<Card>(0, "");
    temp = card;
    //assume parameter "card" is the top (next or the card being played currently) in draw deck 
    drawDeck.pop_back();
    addReserveBackToDraw(); 
    reserveCard = temp;
}

std::shared_ptr<Card> Player::getReserveCard() {
    return reserveCard;
}

void Player::addReserveBackToDraw() {
    if (reserveCard != nullptr ) {      
		addToDraw(reserveCard);     //put reserve card on drawDeck
		reserveCard = nullptr;      //remove reserveCard placeholder
	}
}

int Player::isReserveEmpty() {
    if (reserveCard == nullptr) {
        return 0;
    }
    return 1;
}

void Player::emptyReserve() {
    addReserveBackToDraw();
}

std::shared_ptr<Card> Player::setJokerCard(std::shared_ptr<Card> card, string val) {
    card->assignJoker();
    card->setSym(val);
    if (card->getSym() == "J" || card->getSym() == "11") {
		card->setVal(11);
        card->setSym("J");
	} else if (card->getSym() == "Q" || card->getSym() == "12") {
		card->setVal(12);
        card->setSym("Q");
	} else if (card->getSym() == "K" || card->getSym() == "13") {
		card->setVal(13);
        card->setSym("K");
	} else if (card->getSym() == "A" || card->getSym() == "1") {
		card->setVal(1);
        card->setSym("A");
	} else {
		card->setVal(stoi(val));
	}
    return card;
}

//adding to back of drawDeck, this would make it the card on top (the next card)
void Player::addToDraw(std::shared_ptr<Card> card) {
    drawDeck.emplace_back(card);
}

//it doesn't really matter how I add to discardDeck
void Player::addToDiscard(std::shared_ptr<Card> card) {
    discardDeck.emplace_back(card);
}

std::shared_ptr<Card> Player::drawNext() {
    //if drawDeck is empty reshuffle discard 
    if (drawDeck.size() == 0 && discardDeck.size() > 0) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle (discardDeck.begin(), discardDeck.end(), std::default_random_engine(seed));
        for (auto cards: discardDeck) {                           //loop through to populate the drawDeck with the discardDeck's cards
			drawDeck.emplace_back(discardDeck.back());
			discardDeck.pop_back();                               //empty discardDeck 
		}
    } else if (drawDeck.size() == 0 && discardDeck.size() == 0) {
        return nullptr;
    }
    return drawDeck.back();
}

void Player::shuffleDiscardIntoDraw() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (discardDeck.begin(), discardDeck.end(), std::default_random_engine(seed));
    for (auto cards: discardDeck) {                           //loop through to populate the drawDeck with the discardDeck's cards
        drawDeck.emplace_back(discardDeck.back());
        discardDeck.pop_back();                               //empty discardDeck 
    }
}

void Player::playCard() {
    drawDeck.pop_back();
}

void Player::cutHeads(std::vector<std::shared_ptr<Card>> headPile) {
    addToDiscard(drawNext());   //actually put the played card into discard pile
    playCard();                 //remove played card from the draw pile
    addReserveBackToDraw();     //exactly as the function name says you dumbass :)

    for (auto card : headPile) {                          //loop through to throw out the headPile (the oldest head) that is being cut
        if (card->isAssignedJokerCard()) {                //if find a joker unassign it by turning it back into a joker card
            card->unassignJoker();
        }
        discardDeck.emplace_back(card);                     //by populating the discardDeck
		headPile.pop_back();                                //then removing the headPile (emptying it)
	}

    //WITH THE TOP TWO CARDS CREATE TWO NEW HEADS IDK WE'LL DO THIS LATER
}

int Player::drawDeckSize() {
    return drawDeck.size();
}

int Player::discardDeckSize() {
    return discardDeck.size();
}
