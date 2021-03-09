#ifndef HEAD_H
#define HEAD_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "card.h"
#include "player.h"

class Card;
class Player;

class Head {
    private:
    std::vector<std::shared_ptr<Card>> aGivenHeadPile;

    public:
    Head();
    ~Head();
    int headPileSize();
    std::string topCard();
    int getTopCardVal();
    std::shared_ptr<Card> getTopCard();
    std::string getTopCardSuit();
    void addToHeadPile(std::shared_ptr<Card> card);         //add a card to the top of aGivenHeadPile
    void cutHeads(std::shared_ptr<Player> player);          //a player will cut the head, player class will deal with it
};

#endif
