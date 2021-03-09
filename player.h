#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "card.h"

class Card;

class Player {
    private:
    std::vector<std::shared_ptr<Card>> drawDeck;            //vector of cards for drawDeck
    std::vector<std::shared_ptr<Card>> discardDeck;         //vector of cards for discardDeck
    std::shared_ptr<Card> reserveCard;                      //card for the reserve placeholder thing

    public:
    Player();       //constructor
    ~Player();      //destructor

    //Reserve
    void addToReserve(std::shared_ptr<Card> card);
    void swapWithReserve(std::shared_ptr<Card> card);
    std::shared_ptr<Card> getReserveCard();
    void addReserveBackToDraw();
    int isReserveEmpty();
    void emptyReserve();

    //Joker
    std::shared_ptr<Card> setJokerCard(std::shared_ptr<Card> card, std::string val);

    //player's decks
    void addToDraw(std::shared_ptr<Card> card);
    void addToDiscard(std::shared_ptr<Card> card);
    std::shared_ptr<Card> drawNext();
    void shuffleDiscardIntoDraw();

    //playing the game
    void playCard();
    void cutHeads(std::vector<std::shared_ptr<Card>> headPile);

    //getting vector sizes for drawDeck and discardDeck
    int drawDeckSize();
    int discardDeckSize();
};

#endif
